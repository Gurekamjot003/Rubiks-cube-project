#pragma once
#include "Core/MoveParser.hpp"
#include "Cube solving algorithms/CubeSolverFactory.hpp"
#include "Cube Mechanics/CameraDisplayer.hpp"
#include "Cube Mechanics/CubieColorSetter.hpp"


class CubeController{
private:
    Cube* cube;
    CubeSolver* solver;
    Camera* camera;
    CameraDisplayer* displayer;

public:
    CubeController(int n){
        cube = new Cube(n);
        solver = CubeSolverFactory::get_solver(SolverType::LBL, cube);
        camera = new Camera();
        displayer = new CameraDisplayer(camera);
    }

    void solve(){
        solver->solve();
    }

    void set_solver(SolverType type){
        solver = CubeSolverFactory::get_solver(type, cube);
    }


    void apply_move(string move){
        vector<ICommand*> commands = MoveParser::parse(move, camera, cube);
        for(auto& command: commands){
            command->execute(cube, camera);
        }
    }

    void scramble_cube(){
        cube->scramble_cube();
    }

    void display_cube(){
        displayer->display(cube);
    }

    void set_cubie_color(FaceEnum face, int i, int j, Color color){
        Coordinate target(i,j);
        CubieColorSetter* setter = new CubieColorSetter(camera, cube); 
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
    

    bool is_cube_solvable(){
        if(!cube->is_cube_valid()) return false;
        // check for if cube is solvable or not
        // will apply this later
        return true;
    }
};