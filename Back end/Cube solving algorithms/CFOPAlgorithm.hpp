#pragma once
#include "CubeSolver.hpp"

class CFOPAlgorithm : public CubeSolver
{
public:
    CFOPAlgorithm(Cube *c)
    {
        this->cube = c;
    }
    std::vector<std::string> solve(Cube *cube, Camera *camera) override
    {
        this->cube = cube;
        this->camera = camera;
        // apply cfop algo
        return {};
    }
};