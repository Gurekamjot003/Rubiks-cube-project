#pragma once
#include "Cube Mechanics/MoveTranslator.hpp"
#include "Cube solving algorithms/CubeSolverFactory.hpp"


class CubeController{
private:
    Cube* cube;
    CubeSolver* solver;
    Camera camera;
public:
    CubeController(int n){
        cube = new Cube(n);
        solver = CubeSolverFactory::get_solver(SolverType::LBL, cube);
    }

    void solve(){
        solver->solve();
    }

    void set_solver(SolverType type){
        solver = CubeSolverFactory::get_solver(type, cube);
    }

    void apply_move(string move){
        Move m = MoveTranslator::translate(move, camera);
        cube->apply_move(m);
    }

    void apply_move(Axis axis, int layer, int times){
        cube->apply_move(Move(axis, layer, times));
    }

    void scramble_cube(){
        cube->scramble_cube();
    }

    void rotate_cube(RotateType type){
        camera.rotate(type);
    }

    void display_cube(){
        camera.display(cube);
    }

    bool is_cube_solvable(){
        if(!cube->is_cube_valid()) return false;
        // check for if cube is solvable or not
        // will apply this later
        return true;
    }
};