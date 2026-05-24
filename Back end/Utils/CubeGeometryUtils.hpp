#pragma once
#include "../Cube/Cube.hpp"
#include "../Cube Mechanics/Axis.hpp"
#include "../Cube Mechanics/Direction.hpp"
#include <string>
#include <stdexcept>

class CubeGeometryUtils
{
    static std::set<char> view_rotation_moves;
    static std::map<Axis, std::vector<FaceEnum>> next_face_mp;

public:
    static Axis get_axis_from_face(FaceEnum face)
    {
        if (face == FaceEnum::BACK || face == FaceEnum::FRONT)
        {
            return Axis::Z;
        }
        if (face == FaceEnum::UP || face == FaceEnum::DOWN)
        {
            return Axis::Y;
        }
        if (face == FaceEnum::LEFT || face == FaceEnum::RIGHT)
        {
            return Axis::X;
        }
        return Axis::X; // Should not happen
    }

    static bool is_face_visible(FaceEnum face)
    {
        return (face == FaceEnum::FRONT || face == FaceEnum::UP || face == FaceEnum::RIGHT);
    }

    static int get_layer_from_face(FaceEnum face, Cube *cube)
    {
        int cube_size = cube->get_size();
        if (is_face_visible(face))
            return cube_size - 1;
        return 0;
    }

    static FaceEnum get_opposite_face(FaceEnum face)
    {
        if (face == FaceEnum::FRONT)
            return FaceEnum::BACK;
        if (face == FaceEnum::BACK)
            return FaceEnum::FRONT;
        if (face == FaceEnum::UP)
            return FaceEnum::DOWN;
        if (face == FaceEnum::DOWN)
            return FaceEnum::UP;
        if (face == FaceEnum::RIGHT)
            return FaceEnum::LEFT;
        if (face == FaceEnum::LEFT)
            return FaceEnum::RIGHT;
        return FaceEnum::FRONT; // Should not happen
    }

    static int get_relative_num_rotations(Direction direction)
    {
        if (direction == Direction::CLOCKWISE)
            return 1;
        if (direction == Direction::ANTI_CLOCKWISE)
            return 3;
        if (direction == Direction::DOUBLE_ROTATION)
            return 2;
        return 0; // Should not happen
    }

    static int get_num_rotations(Direction direction, FaceEnum face = FaceEnum::FRONT)
    {
        int relative_num_rotations = get_relative_num_rotations(direction);
        if (!is_face_visible(face))
            return 4 - relative_num_rotations;
        return relative_num_rotations;
    }

    static bool is_view_rotate_move(char c)
    {
        return view_rotation_moves.count(c);
    }

    static bool is_wide_move(std::string &move)
    {
        for (auto &ch : move)
        {
            if (islower(ch) || ch == 'w')
                return true;
        }
        return false;
    }

    static Direction get_direction_from_move(char c)
    {
        if (c == '\'')
            return Direction::ANTI_CLOCKWISE;
        if (c == '2')
            return Direction::DOUBLE_ROTATION;
        return Direction::CLOCKWISE;
    }

    static Coordinate get_abs_coordinates_by_top_left_and_top_right(Coordinate target, Coordinate top_left, Coordinate top_right, Cube *cube)
    {
        int n = cube->get_size();
        if (top_left.get_x() != top_right.get_x())
        {
            int i = target.get_y(), j = target.get_x();
            if (top_left.get_x() != 0)
            {
                i = n - 1 - i;
            }
            if (top_left.get_y() != 0)
            {
                j = n - 1 - j;
            }
            return Coordinate(i, j);
        }
        int i = target.get_x(), j = target.get_y();
        if (top_left.get_x() != 0)
        {
            i = n - 1 - i;
        }
        if (top_left.get_y() != 0)
        {
            j = n - 1 - j;
        }
        return Coordinate(i, j);
    }

    static Coordinate get_abs_coordinates(Axis axis, Coordinate coordinate, Cube *cube)
    {
        int n = cube->get_size();
        int i = coordinate.get_x(), j = coordinate.get_y();
        if (axis == Axis::X)
        {
            return Coordinate(n - 1 - i, n - 1 - j);
        }
        else if (axis == Axis::Y)
        {
            return Coordinate(j, i);
        }
        else if (axis == Axis::Z)
        {
            return Coordinate(n - 1 - j, i);
        }
        return {};
    }

    // this function returns a face on basis of current face, axis of rotation and number of rotations.
    static FaceEnum get_next_face(Axis axis, int num_rotations, FaceEnum cur_face)
    {
        auto &next_faces = next_face_mp[axis];
        int idx = -1;
        for (int i = 0; i < next_faces.size(); i++)
        {
            if (next_faces[i] == cur_face)
            {
                idx = i;
                break;
            }
        }
        if (idx == -1)
            return cur_face;
        int new_idx = (idx + num_rotations) % next_faces.size();
        return next_faces[new_idx];
    }

    static std::vector<std::vector<Cubie *>> get_cubies_by_face_in_2D(FaceEnum face, Cube *cube, Coordinate top_left = Coordinate(0, 0), Coordinate top_right = Coordinate(0, 1))
    {
        Axis axis = CubeGeometryUtils::get_axis_from_face(face);
        int layer = CubeGeometryUtils::get_layer_from_face(face, cube);

        return cube->get_cubies_in_layer(axis, layer, top_left, top_right);
    }

    // This function simply returns all cubies in a 1D vector for a given cube instead of 3D cubies vector.
    // This is useful when we want to search for a cubie with specific colors or faces.
    static std::vector<Cubie*> get_all_cubies(Cube* cube){
        std::vector<Cubie*> all_cubies;
        auto& cubies = cube->get_all_cubies();
        for(auto& plane: cubies){
            for(auto& row: plane){
                for(auto& cubie_ptr: row){
                    if(cubie_ptr) all_cubies.push_back(cubie_ptr.get());
                }
            }
        }
        return all_cubies;
    }

    static std::vector<Cubie *> get_cubies_by_faces_present(const std::vector<FaceEnum> &faces, Cube *cube, CubieType type = CubieType::ANY)
    {

        std::vector<Cubie*> cubies = get_all_cubies(cube);
        std::vector<Cubie *> cubie_pieces;
        for (auto &cubie : cubies){
            if ((type == CubieType::ANY || cubie->get_type() == type) && cubie->check_faces_present(faces))
            {
                cubie_pieces.push_back(cubie);
            }
        }
        return cubie_pieces;
    }

    static std::vector<Cubie *> get_cubies_by_faces_matching(const std::vector<FaceEnum> &faces, Cube *cube)
    {
        std::vector<Cubie *> ans;
        std::vector<Cubie*> cubies = get_all_cubies(cube);
        for (auto &cubie : cubies)         {
            if (cubie->check_faces_match(faces))
            {
                ans.push_back(cubie);
            }
        }
        return ans;
    }

    static std::vector<Cubie *> get_cubies_by_colors_present(const std::vector<Color> &colors, Cube *cube, CubieType type = CubieType::ANY)
    {
        std::vector<Cubie*> cubies = get_all_cubies(cube);
        std::vector<Cubie *> cubie_pieces;
        for (auto &cubie : cubies){
            if ((type == CubieType::ANY || cubie->get_type() == type) && cubie->check_colors_present(colors))
            {
                cubie_pieces.push_back(cubie);
            }
        }
        return cubie_pieces;
    }

    static std::vector<Cubie*> get_cubies_by_colors_matching(const std::vector<Color>& colors, Cube* cube){
        std::vector<Cubie*> ans;
        std::vector<Cubie*> cubies = get_all_cubies(cube);
        for(auto& cubie: cubies){
            if(cubie->check_colors_match(colors)){
                ans.push_back(cubie);
            }
        }
        return ans;
    }

    static Color get_face_color(Cube* cube, FaceEnum face){
        std::vector<Cubie*> face_middle = CubeGeometryUtils::get_cubies_by_faces_matching({face}, cube);
        if(face_middle.empty())
            return Color::EMPTY;
        return face_middle[0]->get_color_from_face(face);
    }

    static bool check_cubie_position(Cubie* cubie, Cube* cube){
        std::map<FaceEnum, Color> colors = cubie->get_colors();
        for(auto&[face, color]: colors){
            if(color != CubeGeometryUtils::get_face_color(cube, face)){
                return false;
            }
        }
        return true;
    }

    static bool check_cube_solved(Cube* cube){
        std::vector<Cubie*> cubies = get_all_cubies(cube);
        for(auto& cubie: cubies){
            if(!check_cubie_position(cubie, cube)){
                return false;
            }
        }
        return true;
    }
};