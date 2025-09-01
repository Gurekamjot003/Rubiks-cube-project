#pragma once
#include "../Cube/FaceEnum.hpp"
#include "../Cube/Cube.hpp"
#include "../Utils/CubeGeometryUtils.hpp"
#include "RotationMove.hpp"

class CameraRotator;
class Camera{
    FaceEnum front_face, up_face, right_face;
    
public:
    Camera(){
        front_face = FaceEnum::FRONT;
        up_face = FaceEnum::UP;
        right_face = FaceEnum::RIGHT;
    }

    const FaceEnum get_front_face() &{
        return front_face;
    }

    const FaceEnum get_up_face() &{
        return up_face;
    }

    const FaceEnum get_right_face() &{
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

    std::vector<FaceEnum> get_orientation(){
        return {front_face, right_face, up_face};
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