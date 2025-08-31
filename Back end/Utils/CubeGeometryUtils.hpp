#pragma once
#include "../Cube/Cube.hpp"
#include "../Cube Mechanics/Axis.hpp"
#include "../Cube Mechanics/Direction.hpp"

class CubeGeometryUtils{
    static set<char> view_rotation_moves;
    static map<Axis, vector<FaceEnum>> next_face_mp;
public:
    static Axis get_axis_from_face(FaceEnum face){
        if(face == FaceEnum::BACK or face == FaceEnum::FRONT){
            return Axis::Z;
        }
        if(face == FaceEnum::UP or face == FaceEnum::DOWN){
            return Axis::Y;
        }
        if(face == FaceEnum::LEFT or face == FaceEnum::RIGHT){
            return Axis::X;
        }
        return Axis::X; // Should not happen
    }

    static bool is_face_visible(FaceEnum face){
        return (face == FaceEnum::FRONT or face == FaceEnum::UP or face == FaceEnum::RIGHT);
    }

    static int get_layer_from_face(FaceEnum face, Cube* cube){
        int cube_size = cube->get_size();
        if(is_face_visible(face)) return cube_size-1;
        return 0; 
    }

    static FaceEnum get_opposite_face(FaceEnum face){
        if(face == FaceEnum::FRONT) return FaceEnum::BACK;        
        if(face == FaceEnum::BACK) return FaceEnum::FRONT;
        if(face == FaceEnum::UP) return FaceEnum::DOWN;
        if(face == FaceEnum::DOWN) return FaceEnum::UP;
        if(face == FaceEnum::RIGHT) return FaceEnum::LEFT;
        if(face == FaceEnum::LEFT) return FaceEnum::RIGHT;
        return FaceEnum::FRONT; // Should not happen
    }

    static int get_relative_num_rotations(Direction direction){
        if(direction == Direction::CLOCKWISE) return 1;
        if(direction == Direction::ANTI_CLOCKWISE) return 3;
        if(direction == Direction::DOUBLE_ROTATION) return 2;
        return 0; // Should not happen
    
    }

    static int get_num_rotations(Direction direction, FaceEnum face = FaceEnum::FRONT){
        int relative_num_rotations = get_relative_num_rotations(direction);
        if(!is_face_visible(face)) return 4 - relative_num_rotations;
        return relative_num_rotations;
    }

    static bool is_view_rotate_move(char c){
        return view_rotation_moves.count(c);
    }

    static bool is_wide_move(string& move){
        for(auto& ch: move){
            if(islower(ch) or ch == 'w') return true;
        }
        return false;
    }

    static Axis get_axis_from_rotation_move(char c){
        if(!is_view_rotate_move(c)) throw runtime_error("character is not a valid rotation move");
        if(c == 'x') return Axis::X;
        if(c == 'y') return Axis::Y;
        if(c == 'z') return Axis::Z;
        return Axis::X;
    }

    static Direction get_direction_from_move(char c){
        if(c == '\'') return Direction::ANTI_CLOCKWISE;
        if(c == '2') return Direction::DOUBLE_ROTATION;
        return Direction::CLOCKWISE;
    }

    static Coordinate get_abs_coordinates_by_top_left_and_top_right(Coordinate target, Coordinate top_left, Coordinate top_right, Cube* cube){
        int n = cube->get_size();
        if(top_left.get_x() != top_right.get_x()){
            int i = target.get_y(), j = target.get_x();
            if(top_left.get_x() != 0){
                i = n-1-i;
            }
            if(top_left.get_y() != 0){
                j = n-1-j;
            }        
            return Coordinate(i, j);
        }
        int i = target.get_x(), j = target.get_y();
        if(top_left.get_x() != 0){
            i = n-1-i;
        }
        if(top_left.get_y() != 0){
            j = n-1-j;
        }        
        return Coordinate(i, j);
    }

    static Coordinate get_abs_coordinates(Axis axis, Coordinate coordinate, Cube* cube){
        int n = cube->get_size();
        int i = coordinate.get_x(), j = coordinate.get_y();
        if(axis == Axis::X){
            return Coordinate(n-1-i, n-1-j);
        }
        else if(axis == Axis::Y){
            return Coordinate(j, i);
        }
        else if(axis == Axis::Z){
            return Coordinate(n-1-j, i);
        }
        return {};
    }

    static FaceEnum get_next_face(Axis axis, int num_rotations, FaceEnum cur_face){
        auto& next_faces = next_face_mp[axis];
        int idx = -1;
        for(int i = 0; i<next_faces.size(); i++){
            if(next_faces[i] == cur_face){
                idx = i;
                break;
            }
        }
        if(idx == -1) return cur_face;
        int new_idx = (idx+num_rotations)%next_faces.size(); 
        return next_faces[new_idx];
    }

    static vector<vector<Cubie*>> get_cubies_by_face(FaceEnum face, Cube* cube, Coordinate top_left = Coordinate(0,0), Coordinate top_right = Coordinate(0, 1)){
        Axis axis = CubeGeometryUtils::get_axis_from_face(face);
        int layer = CubeGeometryUtils::get_layer_from_face(face, cube);

        return cube->get_cubies_in_layer(axis, layer, top_left, top_right);
    }
};

vector<char> rotation_moves_arr = {'x', 'y', 'z'};
set<char> CubeGeometryUtils::view_rotation_moves = set<char>(rotation_moves_arr.begin(), rotation_moves_arr.end());
map<Axis, vector<FaceEnum>> CubeGeometryUtils::next_face_mp = {
    {Axis::X, {
        FaceEnum::UP,
        FaceEnum::BACK,
        FaceEnum::DOWN,
        FaceEnum::FRONT
    }},
    {Axis::Y, {
        FaceEnum::FRONT,
        FaceEnum::LEFT,
        FaceEnum::BACK,
        FaceEnum::RIGHT
    }},
    {Axis::Z, {
        FaceEnum::UP,
        FaceEnum::RIGHT,
        FaceEnum::DOWN,
        FaceEnum::LEFT
    }}
};



void CubieRotator::rotate_cubie(Cubie* cubie, RotationMove& move){
    Axis axis = move.get_axis();
    int num_rotations = move.get_times();

    map<FaceEnum, Color> new_colors;
    for(auto[face, color]: cubie->get_colors()){
        FaceEnum new_face =  CubeGeometryUtils::get_next_face(axis, num_rotations, face);
        new_colors[new_face] = color;
    }
    cubie->set_colors(new_colors);
}