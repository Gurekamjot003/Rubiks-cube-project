#pragma once
#include "Color.hpp"
#include "FaceEnum.hpp"
#include "CubieType.hpp"
#include "../Cube Mechanics/Axis.hpp"
#include "../Cube Mechanics/RotationMove.hpp"
#include <map>
#include <vector>
using namespace std;


class Move;
class CubieRotator;
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

    map<FaceEnum, Color> get_colors(){
        return colors;
    }

    void set_colors(map<FaceEnum, Color> new_colors){
        colors = new_colors;
    }

    CubieType get_type(){
        return type;
    }

    Color get_color_from_face(FaceEnum face){
        if(colors.find(face) == colors.end()) return Color::EMPTY;
        return colors[face];
    }

    bool check_faces_present(vector<FaceEnum> faces){
        if(faces.size() != colors.size()) return false;
        for(auto& face: faces){
            if(colors.find(face) == colors.end()) return false;
        }
        return true;
    }

    void set_color_by_face(FaceEnum face, Color color){
        colors[face] = color;
    }
};

class CubeGeometryUtils;
class CubieRotator{
    Cubie* cubie;
public: 
    CubieRotator(Cubie* cubie): cubie(cubie){}

    
    void rotate_cubie(RotationMove& move){
        Axis axis = move.get_axis();
        int num_rotations = move.get_times();

        map<FaceEnum, Color> new_colors;
        for(auto[face, color]: cubie->get_colors()){
            FaceEnum new_face =  CubeGeometryUtils::get_next_face(axis, num_rotations, face);
            new_colors[new_face] = color;
        }
        cubie->set_colors(new_colors);
    }                                                                                                                                               
};