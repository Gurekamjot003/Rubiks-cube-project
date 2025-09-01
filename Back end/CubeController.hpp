#pragma once
#include "Core/MoveParser.hpp"
#include "Cube solving algorithms/CubeSolverFactory.hpp"
#include "Cube Mechanics/CameraDisplayer.hpp"
#include "Cube Mechanics/CubieColorSetter.hpp"


class CubeController{
private:
    unique_ptr<Cube> cube;
    unique_ptr<CubeSolver> solver;
    unique_ptr<Camera> camera;
    unique_ptr<CameraDisplayer> displayer;

public:
    CubeController(int n){
        cube = make_unique<Cube>(n);
        solver = CubeSolverFactory::get_solver(SolverType::LBL, cube.get());
        camera = make_unique<Camera>();
        displayer = make_unique<CameraDisplayer>(camera.get());
    }

    void solve(){
        solver->solve();
    }

    void set_solver(SolverType type){
        solver = CubeSolverFactory::get_solver(type, cube.get());
    }


    void apply_move(string move){
        vector<unique_ptr<ICommand>> commands = MoveParser::parse(move, camera.get(), cube.get());
        for(auto& command: commands){
            command->execute(cube.get(), camera.get());
        }
    }

    void scramble_cube(){
        cube->scramble_cube();
    }

    void display_cube(){
        displayer->display(cube.get());
    }

    void set_cubie_color(FaceEnum face, int i, int j, Color color){
        Coordinate target(i,j);
        unique_ptr<CubieColorSetter> setter = make_unique<CubieColorSetter>(camera.get(), cube.get()); 
        setter->set_color(face, target, color);
    }

    FaceEnum get_front_face(){
        return camera->get_front_face();
    }

    FaceEnum get_up_face(){
        return camera->get_up_face();
    }
    
    FaceEnum get_right_face(){
        return camera->get_right_face();
    }
    
    bool is_cube_solved(){
        return cube->is_solved();
    }

    bool is_cube_valid(){
        return cube->is_cube_valid();
    }

    bool is_cube_solvable(){
        if(!cube->is_cube_valid()) return false;
        // check for if cube is solvable or not
        // will apply this later
        return true;
    }
};