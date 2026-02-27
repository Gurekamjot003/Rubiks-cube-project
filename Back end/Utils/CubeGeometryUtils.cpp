#include "CubeGeometryUtils.hpp"

std::vector<char> rotation_moves_arr = {'x', 'y', 'z'};
std::set<char> CubeGeometryUtils::view_rotation_moves = std::set<char>(rotation_moves_arr.begin(), rotation_moves_arr.end());
std::map<Axis, std::vector<FaceEnum>> CubeGeometryUtils::next_face_mp = {
    {Axis::X, {
        FaceEnum::UP,
        FaceEnum::BACK,
        FaceEnum::DOWN,
        FaceEnum::FRONT
    }},
    {Axis::Y, {
        FaceEnum::FRONT,
        FaceEnum::LEFT,
        FaceEnum::BACK,
        FaceEnum::RIGHT
    }},
    {Axis::Z, {
        FaceEnum::UP,
        FaceEnum::RIGHT,
        FaceEnum::DOWN,
        FaceEnum::LEFT
    }}
};
