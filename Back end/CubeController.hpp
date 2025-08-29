#pragma once
#include "Core/MoveParser.hpp"
#include "Cube solving algorithms/CubeSolverFactory.hpp"


class CubeController{
private:
    Cube* cube;
    CubeSolver* solver;
    Camera* camera;

public:
    CubeController(int n){
        cube = new Cube(n);
        solver = CubeSolverFactory::get_solver(SolverType::LBL, cube);
        camera = new Camera();
    }

    void solve(){
        solver->solve();
    }

    void set_solver(SolverType type){
        solver = CubeSolverFactory::get_solver(type, cube);
    }


    void apply_move(string& move){
        vector<ICommand*> commands = MoveParser::parse(move, camera, cube);
        for(auto& command: commands){
            command->execute(cube, camera);
        }
    }

    void scramble_cube(){
        cube->scramble_cube();
    }


    void display_cube(){
        camera->display(cube);
    }

    bool is_cube_solvable(){
        if(!cube->is_cube_valid()) return false;
        // check for if cube is solvable or not
        // will apply this later
        return true;
    }
};