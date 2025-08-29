#pragma once
#include  "../Cube/CubeRotator.hpp"
#include "../Cube Mechanics/Camera.hpp"
#include "../Cube Mechanics/Direction.hpp"

class ICommand{
public:    
    virtual void execute(Cube* cube, Camera* camera) = 0;
};