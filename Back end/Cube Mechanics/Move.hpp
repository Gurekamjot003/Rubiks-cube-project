#pragma once
#include "Axis.hpp"

class Move{
private:
    Axis axis;
    int layer;
    int times;
public:
    Move(Axis axis, int layer, int times){
        this->axis = axis;
        this->layer = layer;
        this->times = times;
    }

    Axis get_axis(){
        return axis;
    }

    int get_layer(){
        return layer;
    }

    int get_times(){
        return times;
    }
};