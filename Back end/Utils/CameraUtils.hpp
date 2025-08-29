#pragma once
#include "../Cube Mechanics/Camera.hpp"
#include "CubeGeometryUtils.hpp"

class CameraUtils{
public:
    static FaceEnum get_face_enum_from_move(char move, Camera* camera){
        if(move == 'r') return camera->get_right_face();
        if(move == 'l') return CubeGeometryUtils::get_opposite_face(camera->get_right_face());
        if(move == 'u') return camera->get_up_face();
        if(move == 'd') return CubeGeometryUtils::get_opposite_face(camera->get_up_face());
        if(move == 'f') return camera->get_front_face();
        if(move == 'b') return CubeGeometryUtils::get_opposite_face(camera->get_front_face());
        return FaceEnum::FRONT;
    }

    static int get_num_rotations(Direction direction){
        int num_rotations = CubeGeometryUtils::get_relative_num_rotations(direction);
        // camera will rotate opposite to cube
        return 4-num_rotations;
    }
};