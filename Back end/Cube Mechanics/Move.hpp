#pragma once
#include "Axis.hpp"

class Move{
private:
    Axis axis;
    int layer;
    int times;
public:
    Move(){
        
    }
    Move(Axis axis, int layer, int times){
        this->axis = axis;
        this->layer = layer;
        this->times = times;
    }

    const Axis get_axis() &{
        return axis;
    }

    const int get_layer() &{
        return layer;
    }

    const int get_times() &{
        return times;
    }
};

