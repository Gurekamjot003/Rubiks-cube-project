#pragma once
#include "RotateType.hpp"
#include "../Cube/FaceEnum.hpp"
#include "../Cube/Cube.hpp"

class Camera{
    FaceEnum front_face, up_face, right_face;

    
public:
    Camera(){
        front_face = FaceEnum::FRONT;
        up_face = FaceEnum::UP;
        right_face = FaceEnum::RIGHT;
    }

    void rotate(RotateType type){
        if(type == RotateType::LEFT){
            FaceEnum temp = front_face;
            front_face = get_opposite_face(right_face);
            right_face = temp;
        }
        else if(type == RotateType::RIGHT){
            FaceEnum temp = front_face;
            front_face = right_face;
            right_face = get_opposite_face(temp);
        }
        else if(type == RotateType::UP){
            FaceEnum temp = front_face;
            front_face = up_face;
            up_face = get_opposite_face(temp);
        }
        else if(type == RotateType::DOWN){
            FaceEnum temp = front_face;
            front_face = get_opposite_face(up_face);
            up_face = temp;
        }
        else if(type == RotateType::CLOCKWISE){
            FaceEnum temp = up_face;
            up_face = get_opposite_face(right_face);
            right_face = temp;
        }
        else if(type == RotateType::ANTICLOCKWISE){
            FaceEnum temp = up_face;
            up_face = right_face;
            right_face = get_opposite_face(temp);
        }
    }

    FaceEnum get_front_face(){
        return front_face;
    }

    FaceEnum get_up_face(){
        return up_face;
    }

    FaceEnum get_right_face(){
        return right_face;
    }

    vector<FaceEnum> get_orientation(){
        return {front_face, right_face, up_face};
    }

    vector<Color> get_up_colors(vector<Cubie*> up_cubies){
        FaceEnum back_face = get_opposite_face(front_face);
        FaceEnum left_face = get_opposite_face(right_face);
        FaceEnum down_face = get_opposite_face(up_face);

        vector<vector<FaceEnum>> req_faces = {
            {up_face, back_face, left_face},
            {up_face, back_face},
            {up_face, back_face, right_face},
            {up_face, left_face},
            {up_face},
            {up_face, right_face},
            {up_face, front_face, left_face},
            {up_face, front_face},
            {up_face, front_face, right_face}
        };

        vector<Color> ans;
        for(auto& cur_faces_req: req_faces)
        for(auto& cubie: up_cubies){
            if(cubie->faces_present(cur_faces_req)) 
                ans.push_back(cubie->get_color(up_face));
        }

        return ans;
    }

    vector<Color> get_front_colors(vector<Cubie*> front_cubies){
        FaceEnum back_face = get_opposite_face(front_face);
        FaceEnum left_face = get_opposite_face(right_face);
        FaceEnum down_face = get_opposite_face(up_face);

        vector<vector<FaceEnum>> req_faces = {
            {front_face, up_face, left_face},
            {front_face, up_face},
            {front_face, up_face, right_face},
            {front_face, left_face},
            {front_face},
            {front_face, right_face},
            {front_face, down_face, left_face},
            {front_face, down_face},
            {front_face, down_face, right_face}
        };

        vector<Color> ans;
        for(auto& cur_faces_req: req_faces)
        for(auto& cubie: front_cubies){
            if(cubie->faces_present(cur_faces_req)) 
                ans.push_back(cubie->get_color(front_face));
        }

        return ans;
    }

    vector<Color> get_right_face(vector<Cubie*> right_cubies){
        FaceEnum back_face = get_opposite_face(front_face);
        FaceEnum left_face = get_opposite_face(right_face);
        FaceEnum down_face = get_opposite_face(up_face);

        vector<vector<FaceEnum>> req_faces = {
            {right_face, up_face, front_face},
            {right_face, up_face},
            {right_face, up_face, back_face},
            {right_face, front_face},
            {right_face},
            {right_face, back_face},
            {right_face, down_face, front_face},
            {right_face, down_face},
            {right_face, down_face, back_face}
        };

        vector<Color> ans;
        for(auto& cur_faces_req: req_faces)
        for(auto& cubie: right_cubies){
            if(cubie->faces_present(cur_faces_req)) 
                ans.push_back(cubie->get_color(right_face));
        }

        return ans;
        
    }

    char get_color_char(Color color){
        if(color == Color::BLUE) return 'B';
        if(color == Color::GREEN) return 'G';
        if(color == Color::RED) return 'R';
        if(color == Color::ORANGE) return 'O';
        if(color == Color::WHITE) return 'W';
        if(color == Color::YELLOW) return 'Y';
        return 'X';
    }

    void display(Cube* cube){

        cout<<"\n\n--------Current cube state---------\n\n";
        
       
        vector<Cubie*> up_cubies = cube->get_cubies_by_face(up_face);
        vector<Cubie*> front_cubies = cube->get_cubies_by_face(front_face);
        vector<Cubie*> right_cubies = cube->get_cubies_by_face(right_face);
        
        vector<Color> up_colors = get_up_colors(up_cubies);
        int it = 0;
        for(; it<up_cubies.size(); ){
            for(int i = 0; i<cube->get_size(); i++){
                cout<<get_color_char(up_colors[it+i])<<" ";
            }
            cout<<endl;
            it+=cube->get_size();
        }

        cout<<endl;
        vector<Color> front_colors = get_front_colors(front_cubies);
        vector<Color> right_colors = get_right_face(right_cubies);
        it = 0;
        for(;it<front_colors.size();){
            for(int i = 0; i<cube->get_size(); i++){
                cout<<get_color_char(front_colors[it+i])<<" ";
            }
            cout<<"\t";
            for(int i = 0; i<cube->get_size(); i++){
                cout<<get_color_char(right_colors[it+i])<<" ";
            }
            cout<<endl;
            it += cube->get_size();
        }
    }
};