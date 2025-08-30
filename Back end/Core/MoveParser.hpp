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
        static ICommand* parse_token(string& token, Camera* camera, Cube* cube){
            Axis axis = CubeGeometryUtils::get_axis_from_rotation_move(token[0]);
            Direction direction = CubeGeometryUtils::get_direction_from_move(token.back());
            int num_rotations = CameraUtils::get_num_rotations(direction);
            return new ViewRotateCommand(axis, num_rotations);
        }
    };

    class FaceTurnParser{
    public:     
        static ICommand* parse_token(string& token, Camera* camera, Cube* cube){
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

            return new FaceTurnCommand(axis, layer, num_rotations);
        }
    };

    class WideFaceTurnParser{
    public:     
        static ICommand* parse_token(string& token, Camera* camera, Cube* cube){
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

            return new WideFaceTurnCommand(axis, layer, num_rotations, num_layers);
        }
    };

    class TokenStandardizer{
    public:
        static void standardize(string& token, Cube* cube){
            bool is_even_cube = (cube->get_size() % 2 == 0);
            int mid_layer_int = cube->get_size()/2 + 1;
            string ans;
            
            string mid_layer = to_string(mid_layer_int);
            for(auto& ch: token){
                if(tolower(ch) == 'm'){
                    if(is_even_cube) throw invalid_argument("invalid move - even cubes don't have this move");
                    ans += mid_layer + "L";
                }
                else if(tolower(ch) == 'e'){
                    if(is_even_cube) throw invalid_argument("invalid move - even cubes don't have this move");
                    ans += mid_layer+"D";
                }
                else if(tolower(ch) == 's'){
                    if(is_even_cube) throw invalid_argument("invalid move - even cubes don't have this move");
                    ans += mid_layer+"F";
                }
                else ans.push_back(ch);
            }
            token = ans;
        }
    };

public:

    static vector<ICommand*> parse(string& move, Camera* camera, Cube* cube){
        string token = "";
        move.push_back(' ');
        vector<ICommand*> commands;
        for(auto& ch: move){
            if(isspace(ch) && !token.empty()){
                ICommand* cur_command = parse_token(token, camera, cube);
                commands.push_back(cur_command);
                token = "";
            }
            else token.push_back(ch);
        }
        move.pop_back();
        return commands;
    }

    static ICommand* parse_token(string& token, Camera* camera, Cube* cube){

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
