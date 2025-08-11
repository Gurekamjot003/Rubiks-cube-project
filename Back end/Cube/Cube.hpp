#pragma once
#include "Face.hpp"
#include "FaceEnum.hpp"
#include "../Cube Mechanics/Move.hpp"

class Cube{
    
private:
    map<FaceEnum, Face*> faces;
    bool is_solved;

public:
    Cube(int n){
        is_solved = true;
        for(int i=0; i<6; ++i){
            faces.insert(pair<FaceEnum, Face*>(static_cast<FaceEnum>(i), new Face(static_cast<Color>(i), n)));
        }
    }

    void set_color(int x, int y, Color color){
        faces[FaceEnum::FRONT]->set_color(x, y, color);
    }

    void scramble_cube(){
        //scramble cube
    }

    void apply_move(Move* move){
        //apply move
    }
    
    void rotate(Axis axis, int times){
        //rotate cube
    }

    map<FaceEnum, Face*> get_faces(){
        return faces;
    }
    
    void set_faces(map<FaceEnum, Face*> faces){
        this->faces = faces;
    }

    bool is_cube_valid(){
        // color freq should be n*n 
        for(auto&[key, face]: faces){
            map<Color, int> color_freq;
            for(int i=0; i<face->get_face().size(); ++i){                for(int j=0; j<faces[key]->get_face().size(); ++j){
                    color_freq[face->get_color(i,j)]++;
                }
            }
            for(auto&[color, freq]: color_freq){
                if(freq != face->get_length() * face->get_width()){
                    return false;
                }
            }
        }

        // will apply these later
        // edge pieces count should be correct
        

        // corner pieces count should be correct

    }

};