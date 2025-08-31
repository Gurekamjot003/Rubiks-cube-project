#pragma once
#include "../Utils/CameraUtils.hpp"

class CubieColorSetter{
    Camera* camera; 
    Cube* cube;

public: 
    CubieColorSetter(Camera* camera, Cube* cube): camera(camera), cube(cube){}
    
    void set_color(FaceEnum face, Coordinate target, Color color){
        Coordinate abs_target = CameraUtils::get_abs_coordinate_for_target(face, camera, target, cube);
        Axis axis = CubeGeometryUtils::get_axis_from_face(face);
        int layer = CubeGeometryUtils::get_layer_from_face(face, cube);
        cube->set_color_by_abs_coordinates(face, axis, layer, abs_target, color);
    }
};