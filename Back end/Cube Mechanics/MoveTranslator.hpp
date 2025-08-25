#pragma once
#include "../Cube/Cube.hpp"
#include "Camera.hpp"

Axis get_axis_from_face(FaceEnum face){
    if(face == FaceEnum::BACK or face == FaceEnum::FRONT){
        return Axis::Z;
    }
    if(face == FaceEnum::UP or face == FaceEnum::DOWN){
        return Axis::Y;
    }
    if(face == FaceEnum::LEFT or face == FaceEnum::RIGHT){
        return Axis::X;
    }
    return Axis::X; // Should not happen
}



int get_layer_from_face(FaceEnum face){
    if(face == FaceEnum::FRONT) return 2;
    if(face == FaceEnum::BACK) return 0;
    if(face == FaceEnum::UP) return 2;
    if(face == FaceEnum::DOWN) return 0;
    if(face == FaceEnum::LEFT) return 0;
    if(face == FaceEnum::RIGHT) return 2;
    return 0; // Should not happen
}

class MoveTranslator{
public:
    static Move translate(string move, Camera camera){
        if(move.empty()) return Move();
        
        FaceEnum front_face = camera.get_front_face();        
        FaceEnum up_face = camera.get_up_face();
        FaceEnum right_face = camera.get_right_face();
        

        // assuming bottom left front corner to be origin
        Axis axis;
        int layer;
        int times;
        switch (tolower(move[0]))
        {
        case 'r':
            axis = get_axis_from_face(right_face);
            layer = get_layer_from_face(right_face);
            times = 1;
            break;
        case 'u':
            axis = get_axis_from_face(up_face);
            layer = get_layer_from_face(up_face);
            times = 1;
            break;
        case 'f':
            axis = get_axis_from_face(front_face);
            layer = get_layer_from_face(front_face);
            times = 1;
            break;

        case 'l':
            axis = get_axis_from_face(right_face); // right axis same as left
            layer = get_layer_from_face(get_opposite_face(right_face));
            times = 1;
            break;
        case 'd':
            axis = get_axis_from_face(up_face);
            layer = get_layer_from_face(get_opposite_face(up_face));
            times = 1;
            break;
        case 'b':
            axis = get_axis_from_face(front_face);
            layer = get_layer_from_face(get_opposite_face(front_face));
            times = 1;
            break;
        default:
            return Move();
        }

        if(move.length() > 1){
            switch (move[1])
            {
            case '\'':
                times = -1;
                break;
            case '2':
                times = 2;
                break;
            default:
                return Move();
            }
        }

        return Move(axis, layer, times);
    }
};


void Cubie::rotate_cubie(Axis axis, int layer, int num_rotations){
        
    map<FaceEnum, Color> new_colors;
    for(auto[face, color]: colors){
        FaceEnum new_face;
        
        auto& next_face = next_face_mp[axis];
        int idx = -1;
        for(int i = 0; i<next_face.size(); i++){
            if(next_face[i] == face){
                idx = i; break;
            }
        }
        if(idx == -1) new_face = face; 
        else new_face = next_face[(idx + num_rotations)%next_face.size()];

        new_colors[new_face] = color;
    }

    colors = new_colors;
}