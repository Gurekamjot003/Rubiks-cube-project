#include "CubeRotator.hpp"
#include "Cubie.hpp"
#include "../Cube Mechanics/RotationMove.hpp"
#include "../Utils/CubeGeometryUtils.hpp"

void CubieRotator::rotate_cubie(RotationMove& move){
    Axis axis = move.get_axis();
    int num_rotations = move.get_times();

    std::map<FaceEnum, Color> new_colors;
    for(auto const& [face, color] : cubie->get_colors()){
        FaceEnum new_face =  CubeGeometryUtils::get_next_face(axis, num_rotations, face);
        new_colors[new_face] = color;
    }
    cubie->set_colors(new_colors);
}
