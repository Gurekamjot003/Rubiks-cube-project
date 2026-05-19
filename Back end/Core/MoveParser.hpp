#pragma once
#include "../Utils/CubeGeometryUtils.hpp"
#include "../Utils/CameraUtils.hpp"
#include "FaceTurnCommand.hpp"
#include "WideFaceTurnCommand.hpp"
#include "ViewRotateCommand.hpp"

// singleton
class MoveParser
{

    // parses view/camera rotation moves, for example "x" or "y'" or "z2"
    class ViewRotateParser
    {
    public:
        // returns a view rotation command move based on the token - for example "x" or "y'" or "z2"
        static std::unique_ptr<ICommand> parse_token(std::string &token, Camera *camera, Cube *cube)
        {
            FaceEnum rotating_face = CameraUtils::get_face_enum_from_rotation_move(token[0], camera);
            Axis axis = CubeGeometryUtils::get_axis_from_face(rotating_face);
            Direction direction = CubeGeometryUtils::get_direction_from_move(token.back());
            int num_rotations = CameraUtils::get_num_rotations(direction, rotating_face);
            return std::make_unique<ViewRotateCommand>(axis, num_rotations);
        }
    };


    class FaceTurnParser
    {
        // some moves can be like "3R" or "2U'" where the number indicates layer which is to be turned
        static int get_layer_diff_from_token(std::string &token)
        {
            int layer_diff = 0;
            while (isdigit(token[0]))
            {
                layer_diff = layer_diff * 10 + token[0] - '0';
                token.erase(token.begin());
            }
            if (layer_diff)
                layer_diff -= 1;
            return layer_diff;
        }
    public:
        static std::unique_ptr<ICommand> parse_token(std::string &token, Camera *camera, Cube *cube)
        {
            int layer_diff = get_layer_diff_from_token(token);
            
            FaceEnum rotating_face = CameraUtils::get_face_enum_from_move(tolower(token[0]), camera);
            Axis axis = CubeGeometryUtils::get_axis_from_face(rotating_face);
            int layer = CubeGeometryUtils::get_layer_from_face(rotating_face, cube);
            layer == 0 ? layer += layer_diff : layer -= layer_diff; // get to correct layer based on token
            Direction direction = CubeGeometryUtils::get_direction_from_move(token.back());
            int num_rotations = CubeGeometryUtils::get_num_rotations(direction, rotating_face);

            return std::make_unique<FaceTurnCommand>(axis, layer, num_rotations);
        }
    };

    class WideFaceTurnParser
    {
    public:
        static std::unique_ptr<ICommand> parse_token(std::string &token, Camera *camera, Cube *cube)
        {
            int num_layers = 1;
            if (cube->get_size() > 2)
                num_layers = 2;
            while (isdigit(token[0]))
            {
                num_layers = num_layers * 10 + token[0] - '0';
                token.erase(token.begin());
            }
            FaceEnum rotating_face = CameraUtils::get_face_enum_from_move(tolower(token[0]), camera);
            Axis axis = CubeGeometryUtils::get_axis_from_face(rotating_face);
            int layer = CubeGeometryUtils::get_layer_from_face(rotating_face, cube);
            Direction direction = CubeGeometryUtils::get_direction_from_move(token.back());
            int num_rotations = CubeGeometryUtils::get_num_rotations(direction, rotating_face);

            return std::make_unique<WideFaceTurnCommand>(axis, layer, num_rotations, num_layers);
        }
    };

    class TokenStandardizer
    {
    public:
        // this function standardizes the token by converting moves like "M", "E", "S" to their equivalent face turns like "2L", "2D", "2F" for 3x3 cube
        // this standardization makes it easier to parse the moves later on as we only have to deal with one type of move for face turns instead of multiple notations and illegal moves like "M" for even cubes are also handled here
        static void standardize(std::string &token, Cube *cube)
        {
            bool is_even_cube = (cube->get_size() % 2 == 0);
            int mid_layer_int = cube->get_size() / 2 + 1;
            std::string ans;

            std::string mid_layer = std::to_string(mid_layer_int);
            for (auto &ch : token)
            {
                if (tolower(ch) == 'm')
                {
                    if (is_even_cube)
                        throw std::invalid_argument("invalid move - even cubes don't have this move");
                    ans += mid_layer + "L"; // like "2L" for 3x3 cube
                }
                else if (tolower(ch) == 'e')
                {
                    if (is_even_cube)
                        throw std::invalid_argument("invalid move - even cubes don't have this move");
                    ans += mid_layer + "D"; // like "2D" for 3x3 cube
                }
                else if (tolower(ch) == 's')
                {
                    if (is_even_cube)
                        throw std::invalid_argument("invalid move - even cubes don't have this move");
                    ans += mid_layer + "F"; // like "2F" for 3x3 cube
                }
                else
                    ans.push_back(ch);
            }
            token = ans;
        }
    };

public:
    // this function tokenizes the input move string and returns a vector of commands to execute the move
    static std::vector<std::unique_ptr<ICommand>> parse(std::string &move, Camera *camera, Cube *cube)
    {
        std::string token = "";
        move.push_back(' '); // add a space at the end to ensure the last token is processed
        std::vector<std::unique_ptr<ICommand>> commands;
        for (auto &ch : move)
        {
            if (isspace(ch) && !token.empty()) // process the token at space
            {
                commands.push_back(parse_token(token, camera, cube));
                token = "";
            }
            else
                token.push_back(ch);
        }
        move.pop_back(); // remove the extra space added at the end
        return commands;
    }

    // strategy design pattern for token parsing
    static std::unique_ptr<ICommand> parse_token(std::string &token, Camera *camera, Cube *cube)
    {

        TokenStandardizer::standardize(token, cube);

        if (CubeGeometryUtils::is_view_rotate_move(token[0]))
        {
            return ViewRotateParser::parse_token(token, camera, cube);
        }
        if (CubeGeometryUtils::is_wide_move(token))
        {
            return WideFaceTurnParser::parse_token(token, camera, cube);
        }
        return FaceTurnParser::parse_token(token, camera, cube);
    }
};
