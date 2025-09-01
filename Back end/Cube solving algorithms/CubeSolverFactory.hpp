#pragma once
#include "LBLAlgorithm.hpp"
#include "CFOPAlgorithm.hpp"
#include "KoceimbaAlgorithm.hpp"
#include "SolverType.hpp"


class CubeSolverFactory{
public:
    static unique_ptr<CubeSolver> get_solver(SolverType type, Cube* cube){
        switch (type)        {
        case SolverType::LBL:
            return make_unique<LBLAlgorithm>(cube);
        case SolverType::CFOP:
            return make_unique<CFOPAlgorithm>(cube);
        case SolverType::KOCEIMBA:
            return make_unique<KoceimbaAlgorithm>(cube);
        default:
            return nullptr;
        }
    }
    
};