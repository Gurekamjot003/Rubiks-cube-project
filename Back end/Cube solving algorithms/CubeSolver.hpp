//abstract
#pragma once
#include "../Cube/Cube.hpp"

class CubeSolver{    
protected:
    Cube* cube;
public:
    CubeSolver(Cube* cube) : cube(cube) {}
    virtual void solve() = 0;
};