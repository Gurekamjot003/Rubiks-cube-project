#pragma once
#include "../Cube/FaceEnum.hpp"
#include "../Cube/Cube.hpp"
#include "../Utils/CubeGeometryUtils.hpp"
#include "RotationMove.hpp"

class CameraRotator;
class Camera{
    FaceEnum front_face, up_face, right_face;
    CameraRotator* camera_rotator;
    
public:
    Camera(){
        front_face = FaceEnum::FRONT;
        up_face = FaceEnum::UP;
        right_face = FaceEnum::RIGHT;
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

    void set_front_face(FaceEnum face){
        front_face = face;
    }

    void set_up_face(FaceEnum face){
        up_face = face;
    }

    void set_right_face(FaceEnum face){
        right_face = face;
    }

    void rotate(RotationMove& move);

    vector<FaceEnum> get_orientation(){
        return {front_face, right_face, up_face};
    }

    vector<Color> get_up_colors(vector<Cubie*> up_cubies){
        FaceEnum back_face = CubeGeometryUtils::get_opposite_face(front_face);
        FaceEnum left_face = CubeGeometryUtils::get_opposite_face(right_face);
        FaceEnum down_face = CubeGeometryUtils::get_opposite_face(up_face);

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
                ans.push_back(cubie->get_color_from_face(up_face));
        }

        return ans;
    }

    vector<Color> get_front_colors(vector<Cubie*> front_cubies){
        FaceEnum back_face = CubeGeometryUtils::get_opposite_face(front_face);
        FaceEnum left_face = CubeGeometryUtils::get_opposite_face(right_face);
        FaceEnum down_face = CubeGeometryUtils::get_opposite_face(up_face);

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
                ans.push_back(cubie->get_color_from_face(front_face));
        }

        return ans;
    }

    vector<Color> get_right_colors(vector<Cubie*> right_cubies){
        FaceEnum back_face = CubeGeometryUtils::get_opposite_face(front_face);
        FaceEnum left_face = CubeGeometryUtils::get_opposite_face(right_face);
        FaceEnum down_face = CubeGeometryUtils::get_opposite_face(up_face);

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
                ans.push_back(cubie->get_color_from_face(right_face));
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
        vector<Color> right_colors = get_right_colors(right_cubies);
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

class CameraRotator{
    Camera* camera;
public: 
    CameraRotator(Camera* camera):camera(camera){}

    void rotate_camera(RotationMove& move){
        Axis axis = move.get_axis();
        int num_rotations = move.get_times();

        FaceEnum front_face = camera->get_front_face();
        FaceEnum new_front_face = CubeGeometryUtils::get_next_face(axis, num_rotations, front_face);
        camera->set_front_face(new_front_face);

        FaceEnum up_face = camera->get_up_face();
        FaceEnum new_up_face = CubeGeometryUtils::get_next_face(axis, num_rotations, up_face);
        camera->set_up_face(new_up_face);

        FaceEnum right_face = camera->get_right_face();
        FaceEnum new_right_face = CubeGeometryUtils::get_next_face(axis, num_rotations, right_face);
        camera->set_right_face(new_right_face);
    }
};

void Camera::rotate(RotationMove& move){
    if(!camera_rotator) camera_rotator = new CameraRotator(this);
    camera_rotator->rotate_camera(move);
}