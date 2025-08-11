#pragma once
#include "../Cube/Cube.hpp"

class MoveTranslator{
public:
    static Move* translate(string move, map<FaceEnum, Face*> faces){
        if(move.empty()) return nullptr;
        

        // assuming left back corner to be origin
        Axis axis;
        int layer;
        int times;
        switch (tolower(move[0]))
        {
        case 'r':
            axis = Axis::X;
            layer = 2;
            times = 1;
            break;
        case 'u':
            axis = Axis::Y;
            layer = 2;
            times = 1;
            break;
        case 'f':
            axis = Axis::Z;
            layer = 2;
            times = 1;
            break;

        case 'l':
            axis = Axis::X;
            layer = 0;
            times = 1;
            break;
        case 'd':
            axis = Axis::Y;
            layer = 0;
            times = 1;
            break;
        case 'b':
            axis = Axis::Z;
            layer = 0;
            times = 1;
            break;
        default:
            return nullptr;
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
                return nullptr;
            }
        }

        return new Move(axis, layer, times);
    }
};