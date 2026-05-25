#pragma once
#include "../Utils/CameraUtils.hpp"

class CubieColorSetter{
    Camera* camera; 
    Cube* cube;

public: 
    CubieColorSetter(Camera* camera, Cube* cube): camera(camera), cube(cube){}
    
    void set_color(FaceEnum face, Coordinate target, Color color){
        std::vector<std::vector<Cubie*>> cubies_on_face = CameraUtils::get_cubies_in_display_order(face, camera, cube);
        Cubie* target_cubie = cubies_on_face[target.get_x()][target.get_y()];
        target_cubie->set_color_by_face(face, color);
    }
};