#pragma once
#include "../Utils/CameraUtils.hpp"
#include <iostream>

class CameraDisplayer{
    Camera* camera;

    char get_color_char(Color color){
        if(color == Color::BLUE) return 'B';
        if(color == Color::GREEN) return 'G';
        if(color == Color::RED) return 'R';
        if(color == Color::ORANGE) return 'O';
        if(color == Color::WHITE) return 'W';
        if(color == Color::YELLOW) return 'Y';
        return 'X';
    }
public:
    CameraDisplayer(Camera* camera): camera(camera){}

    vector<vector<Color>> face_displayer(FaceEnum face_to_display, Cube* cube){
        Coordinate top_left = CameraUtils::get_top_left_coordinate_by_face_and_camera(face_to_display, camera, cube);
        Coordinate top_right = CameraUtils::get_top_right_coordinate_by_face_and_camera(face_to_display, camera, cube);
        vector<vector<Cubie*>> cubies = CubeGeometryUtils::get_cubies_by_face(face_to_display, cube, top_left, top_right);
        vector<vector<Color>> face_colors;
        for(auto& cubie_row: cubies){
            vector<Color> cur_row;
            for(auto& cubie: cubie_row){
                Color color = cubie->get_color_from_face(face_to_display);
                cur_row.push_back(color);
            }
            face_colors.push_back(cur_row);
        }
        return face_colors;
    }

    void display(Cube* cube){
        FaceEnum front_face = camera->get_front_face();
        FaceEnum up_face = camera->get_up_face();
        FaceEnum right_face = camera->get_right_face();

        cout<<"\n\n--------Current cube state---------\n\n";
        
        vector<vector<Color>> up_colors = face_displayer(up_face, cube);
        for(auto& row: up_colors){
            for(auto& color: row){
                cout<<get_color_char(color)<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
        vector<vector<Color>> front_colors = face_displayer(front_face, cube),
            right_colors = face_displayer(right_face, cube);

        for(int i= 0; i<cube->get_size(); i++){
            for(auto& color: front_colors[i]) cout<<get_color_char(color)<<" ";
            cout<<"\t";
            for(auto& color: right_colors[i]) cout<<get_color_char(color)<<" ";
            cout<<endl;
        }
        
    }
};