#pragma once
#include "Color.hpp"
#include "FaceEnum.hpp"
#include "CubieType.hpp"
#include "../Cube Mechanics/Axis.hpp"

class Move;


map<Axis, vector<FaceEnum>> next_face_mp = {
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
class Cubie{
private:
    map<FaceEnum, Color> colors;
    CubieType type;
    
public:
    Cubie(Color c, FaceEnum face) : type(CubieType::MIDDLE) {
        colors[face] = c;
    }

    Cubie(map<FaceEnum, Color> colors){
        this->colors = colors;
        if(colors.size() == 3) type = CubieType::CORNER;
        else if(colors.size() == 2) type = CubieType::EDGE;
        else if(colors.size() == 1) type = CubieType::MIDDLE;
        else type = CubieType::EMPTY;
    }

    CubieType get_type(){
        return type;
    }

    Color get_color(FaceEnum face){
        if(colors.find(face) == colors.end()) return Color::EMPTY;
        return colors[face];
    }

    bool faces_present(vector<FaceEnum> faces){
        if(faces.size() != colors.size()) return false;
        for(auto& face: faces){
            if(colors.find(face) == colors.end()) return false;
        }
        return true;
    }

    void set_color(FaceEnum face, Color color){
        colors[face] = color;
    }

    void rotate_cubie(Axis axis, int layer, int num_rotations);
};