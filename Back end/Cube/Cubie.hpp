#pragma once
#include "Color.hpp"
#include "FaceEnum.hpp"
#include "CubieType.hpp"
#include "../Cube Mechanics/Axis.hpp"
#include <map>
#include <vector>


class CubieRotator;
class Cubie{
private:
    std::map<FaceEnum, Color> colors;
    CubieType type;
    
public:
    Cubie(Color c, FaceEnum face) : type(CubieType::MIDDLE) {
        colors[face] = c;
    }

    Cubie(std::map<FaceEnum, Color> colors){
        this->colors = colors;
        if(colors.size() == 3) type = CubieType::CORNER;
        else if(colors.size() == 2) type = CubieType::EDGE;
        else if(colors.size() == 1) type = CubieType::MIDDLE;
        else type = CubieType::EMPTY;
    }

    std::map<FaceEnum, Color> get_colors() const {
        return colors;
    }

    void set_colors(std::map<FaceEnum, Color> new_colors){
        colors = new_colors;
    }

    CubieType get_type() const {
        return type;
    }

    Color get_color_from_face(FaceEnum face) const {
        if(colors.find(face) == colors.end()) return Color::EMPTY;
        return colors.at(face);
    }

    bool check_faces_present(const std::vector<FaceEnum>& faces) const {
        if(faces.size() != colors.size()) return false;
        for(const auto& face: faces){
            if(colors.find(face) == colors.end()) return false;
        }
        return true;
    }

    void set_color_by_face(FaceEnum face, Color color){
        colors[face] = color;
    }
};

class RotationMove;
class CubieRotator{
    Cubie* cubie;
public: 
    CubieRotator(Cubie* cubie): cubie(cubie){}

    
    void rotate_cubie(RotationMove& move);                                                                                                              
};