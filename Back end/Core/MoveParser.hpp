#pragma once
#include "../Utils/CubeGeometryUtils.hpp"
#include "../Utils/CameraUtils.hpp"
#include "FaceTurnCommand.hpp"
#include "WideFaceTurnCommand.hpp"
#include "ViewRotateCommand.hpp"


//singleton
class MoveParser{
    
    

    class ViewRotateParser{
    public:
        static std::unique_ptr<ICommand> parse_token(std::string& token, Camera* camera, Cube* cube){
            Axis axis = CubeGeometryUtils::get_axis_from_rotation_move(token[0]);
            Direction direction = CubeGeometryUtils::get_direction_from_move(token.back());
            int num_rotations = CameraUtils::get_num_rotations(direction);
            return std::make_unique<ViewRotateCommand>(axis, num_rotations);
        }
    };

    class FaceTurnParser{
    public:     
        static std::unique_ptr<ICommand> parse_token(std::string& token, Camera* camera, Cube* cube){
            int layer_diff = 0;
            if(isdigit(token[0])){
                layer_diff = token[0] - '0' - 1;
                token.erase(token.begin());
            }
            FaceEnum rotating_face = CameraUtils::get_face_enum_from_move(tolower(token[0]), camera);
            Axis axis = CubeGeometryUtils::get_axis_from_face(rotating_face);
            int layer = CubeGeometryUtils::get_layer_from_face(rotating_face, cube);
            layer == 0? layer+= layer_diff: layer -= layer_diff;
            Direction direction = CubeGeometryUtils::get_direction_from_move(token.back());
            int num_rotations = CubeGeometryUtils::get_num_rotations(direction, rotating_face);

            return std::make_unique<FaceTurnCommand>(axis, layer, num_rotations);
        }
    };

    class WideFaceTurnParser{
    public:     
        static std::unique_ptr<ICommand> parse_token(std::string& token, Camera* camera, Cube* cube){
            int num_layers = 1;
            if(cube->get_size()>2) num_layers = 2;
            if(isdigit(token[0])){
                num_layers = token[0] - '0';
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

    class TokenStandardizer{
    public:
        static void standardize(std::string& token, Cube* cube){
            bool is_even_cube = (cube->get_size() % 2 == 0);
            int mid_layer_int = cube->get_size()/2 + 1;
            std::string ans;
            
            std::string mid_layer = std::to_string(mid_layer_int);
            for(auto& ch: token){
                if(tolower(ch) == 'm'){
                    if(is_even_cube) throw std::invalid_argument("invalid move - even cubes don't have this move");
                    ans += mid_layer + "L";
                }
                else if(tolower(ch) == 'e'){
                    if(is_even_cube) throw std::invalid_argument("invalid move - even cubes don't have this move");
                    ans += mid_layer+"D";
                }
                else if(tolower(ch) == 's'){
                    if(is_even_cube) throw std::invalid_argument("invalid move - even cubes don't have this move");
                    ans += mid_layer+"F";
                }
                else ans.push_back(ch);
            }
            token = ans;
        }
    };

public:

    static std::vector<std::unique_ptr<ICommand>> parse(std::string& move, Camera* camera, Cube* cube){
        std::string token = "";
        move.push_back(' ');
        std::vector<std::unique_ptr<ICommand>> commands;
        for(auto& ch: move){
            if(isspace(ch) && !token.empty()){
                commands.push_back(parse_token(token, camera, cube));
                token = "";
            }
            else token.push_back(ch);
        }
        move.pop_back();
        return commands;
    }

    static std::unique_ptr<ICommand> parse_token(std::string& token, Camera* camera, Cube* cube){

        TokenStandardizer::standardize(token, cube);
        
        if(CubeGeometryUtils::is_view_rotate_move(token[0])){
            return ViewRotateParser::parse_token(token, camera, cube);
        }
        if(CubeGeometryUtils::is_wide_move(token)){
            return WideFaceTurnParser::parse_token(token, camera, cube);
        }
        return FaceTurnParser::parse_token(token, camera, cube);
    }

};
