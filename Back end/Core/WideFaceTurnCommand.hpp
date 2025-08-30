#pragma once
#include "ICommand.hpp"

class WideFaceTurnCommand:public ICommand{
private:
    Axis axis;
    int layer;
    int times;
    int num_layers;
public: 
    WideFaceTurnCommand(Axis axis, int layer, int times, int num_layers):axis(axis),layer(layer),times(times), num_layers(num_layers){}

    void execute(Cube* cube, Camera* camera) override{
        vector<Move> moves;
        int d = (layer == 0? 1: -1);
        for(int move_count = 0; move_count<num_layers; move_count++){
            Move move(axis, layer, times);
            moves.push_back(move);
            layer+=d;
        }
        CubeRotator* rotator = new CubeRotator(cube);
        for(auto& move: moves){
            rotator->rotate_cube(move);
        }
    }
};