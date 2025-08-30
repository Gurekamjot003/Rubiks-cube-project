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

    static Coordinate get_top_left_coordinate_by_face_and_camera(FaceEnum required_face, Camera* camera, Cube* cube){
        Axis axis = CubeGeometryUtils::get_axis_from_face(required_face);
        int layer = CubeGeometryUtils::get_layer_from_face(required_face, cube);

        vector<FaceEnum> faces_to_search;
        if(required_face == camera->get_up_face()){
            faces_to_search = {
                camera->get_up_face(),
                CubeGeometryUtils::get_opposite_face(camera->get_right_face()),
                CubeGeometryUtils::get_opposite_face(camera->get_front_face())
            };
        }
        else if(required_face == camera->get_front_face()){
            faces_to_search = {
                camera->get_front_face(),
                camera->get_up_face(),
                CubeGeometryUtils::get_opposite_face(camera->get_right_face())
            };
        }
        else if(required_face == camera->get_right_face()){
            faces_to_search = {
                camera->get_right_face(),
                camera->get_up_face(),
                camera->get_front_face()
            };
        }

        return cube->get_corner_piece_coordinate_with_required_faces(axis, layer, faces_to_search);
    }

    static Coordinate get_top_right_coordinate_by_face_and_camera(FaceEnum required_face, Camera* camera, Cube* cube){
        Axis axis = CubeGeometryUtils::get_axis_from_face(required_face);
        int layer = CubeGeometryUtils::get_layer_from_face(required_face, cube);

        vector<FaceEnum> faces_to_search;
        if(required_face == camera->get_up_face()){
            faces_to_search = {
                camera->get_up_face(),
                camera->get_right_face(),
                CubeGeometryUtils::get_opposite_face(camera->get_front_face())
            };
        }
        else if(required_face == camera->get_front_face()){
            faces_to_search = {
                camera->get_front_face(),
                camera->get_up_face(),
                camera->get_right_face()
            };
        }
        else if(required_face == camera->get_right_face()){
            faces_to_search = {
                camera->get_right_face(),
                camera->get_up_face(),
                CubeGeometryUtils::get_opposite_face(camera->get_front_face())
            };
        }

        return cube->get_corner_piece_coordinate_with_required_faces(axis, layer, faces_to_search);
    }

};

