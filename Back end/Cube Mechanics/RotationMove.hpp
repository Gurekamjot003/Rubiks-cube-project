#pragma once
#include "Axis.hpp"

class RotationMove{
    Axis axis;
    int times;

public:
    RotationMove(Axis axis, int times): axis(axis),times(times){}

    Axis get_axis(){
        return axis;
    }
    int get_times(){
        return times;
    }
};