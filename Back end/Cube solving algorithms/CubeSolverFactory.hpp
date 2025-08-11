#pragma once
#include "LBLAlgorithm.hpp"
#include "CFOPAlgorithm.hpp"
#include "KoceimbaAlgorithm.hpp"
#include "SolverType.hpp"


class CubeSolverFactory{
public:
    static CubeSolver* get_solver(SolverType type, Cube* cube){
        switch (type)        {
        case SolverType::LBL:
            return new LBLAlgorithm(cube);
        case SolverType::CFOP:
            return new CFOPAlgorithm(cube);
        case SolverType::KOCEIMBA:
            return new KoceimbaAlgorithm(cube);
        default:
            return nullptr;
        }
    }
    
};