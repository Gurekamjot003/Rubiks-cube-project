//abstract
#pragma once
#include "../Cube/Cube.hpp"
#include "../Core/ICommand.hpp"
#include "../Utils/CameraUtils.hpp"
#include "../Utils/CubeGeometryUtils.hpp"
#include <vector>
#include <string>

class CubeSolver{    
protected:
    Cube* cube;
    Camera* camera;
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
    }

    virtual std::vector<std::string> solve(Cube* cube, Camera* camera) = 0;
    virtual ~CubeSolver() = default;
};