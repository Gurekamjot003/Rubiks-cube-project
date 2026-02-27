#pragma once
#include "../Cube/Cube.hpp"
#include "./IReport.hpp"


std::string color_to_string(Color c) {
    switch(c) {
        case Color::WHITE: return "WHITE";
        case Color::YELLOW: return "YELLOW";
        case Color::BLUE: return "BLUE";
        case Color::GREEN: return "GREEN";
        case Color::RED: return "RED";
        case Color::ORANGE: return "ORANGE";
    }
    return "TRANSPARENT";
}

std::string face_to_string(FaceEnum f){
    switch(f) {
        case FaceEnum::UP: return "UP";
        case FaceEnum::DOWN: return "DOWN";
        case FaceEnum::LEFT: return "LEFT";
        case FaceEnum::RIGHT: return "RIGHT";
        case FaceEnum::FRONT: return "FRONT";
        case FaceEnum::BACK: return "BACK";
    }
    return "UNKNOWN";
}

class CubeWASMAdapter: public IReport{
private:
    Cube* cube;

public:
    CubeWASMAdapter(Cube* cube): cube(cube){}

    std::string get_cube_state_JSON() const override{
        
        json cubeJson = json::array();
        int size = cube->get_size();
        for(int x=0; x<size; x++) {
            json planeJson = json::array();
            for(int y=0; y<size; y++) {
                json rowJson = json::array();
                for(int z=0; z<size; z++) {
                    
                    // 1. Get the read-only cubie using the public getter
                    const Cubie* cubie = cube->get_cubie(x, y, z);
                    
                    // 2. Read its colors and format to JSON                    
                    json cubieMap = {
                        {"UP", color_to_string(cubie->get_color_from_face(FaceEnum::UP))},
                        {"DOWN", color_to_string(cubie->get_color_from_face(FaceEnum::DOWN))},
                        {"FRONT", color_to_string(cubie->get_color_from_face(FaceEnum::FRONT))},
                        {"BACK", color_to_string(cubie->get_color_from_face(FaceEnum::BACK))},
                        {"LEFT", color_to_string(cubie->get_color_from_face(FaceEnum::LEFT))},
                        {"RIGHT", color_to_string(cubie->get_color_from_face(FaceEnum::RIGHT))}
                    };
                    rowJson.push_back(cubieMap);
                }
                planeJson.push_back(rowJson);
            }
            cubeJson.push_back(planeJson);
        }
        return cubeJson.dump();
    }

};