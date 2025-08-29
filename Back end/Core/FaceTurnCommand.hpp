#pragma once
#include "ICommand.hpp"

class FaceTurnCommand:public ICommand{
private:
    Axis axis;
    int layer;
    int times;
public: 
    FaceTurnCommand(Axis axis, int layer, int times): axis(axis), layer(layer), times(times){}
    
    void execute(Cube* cube, Camera* camera) override{
        Move move(axis, layer, times);
        cube->apply_move(move);
    }
};