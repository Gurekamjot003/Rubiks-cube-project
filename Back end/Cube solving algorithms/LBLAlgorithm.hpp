#pragma once
#include "CubeSolver.hpp"

class LBLAlgorithm: public CubeSolver{
public:
    LBLAlgorithm(Cube* cube) : CubeSolver(cube) {}
    void solve(){
        // apply lbl algo
    }
};