#pragma once
#include "CubeSolver.hpp"

class KoceimbaAlgorithm : public CubeSolver
{
public:
    KoceimbaAlgorithm(Cube *c)
    {
        this->cube = c;
    }
    std::vector<std::string> solve(Cube *cube, Camera *camera) override
    {
        this->cube = cube;
        this->camera = camera;
        // apply koceimba algo
        return {};
    }
};