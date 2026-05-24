//abstract
#pragma once
#include "../Cube/Cube.hpp"
#include "../Core/ICommand.hpp"
#include "../Utils/CameraUtils.hpp"
#include "../Utils/CubeGeometryUtils.hpp"
#include "../Cube Mechanics/CameraDisplayer.hpp"
#include <vector>
#include <string>

class CubeSolver{    
protected:
    Cube* cube;
    Camera* camera;
    // CameraDisplayer* displayer = nullptr;
    std::vector<std::string> moves; 
public:
    CubeSolver() {}

    void apply_move(std::string move){
        std::vector<std::unique_ptr<ICommand>> commands = MoveParser::parse(move, camera, cube);
        for (auto &command : commands)
        {
            command->execute(cube, camera);
        }
        moves.push_back(move);
        // if(displayer){
        //     std::cout<<"Applied move: "<<move<<std::endl;
        //     displayer->display(cube);
        // }
    }

    virtual std::vector<std::string> solve(Cube* cube, Camera* camera) = 0;
    virtual ~CubeSolver() = default;
};