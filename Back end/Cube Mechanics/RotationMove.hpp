#pragma once
#include "Axis.hpp"

class RotationMove{
    Axis axis;
    int times;

public:
    RotationMove(Axis axis, int times): axis(axis),times(times){}

    const Axis get_axis() &{
        return axis;
    }
    const int get_times() &{
        return times;
    }
};