#pragma once
#include "../Cube/Cube.hpp"
#include "./IReport.hpp"
#include <string>

// Helper function to convert Color enum to a single character
char color_to_char(Color c) {
    switch(c) {
        case Color::WHITE: return 'W';
        case Color::YELLOW: return 'Y';
        case Color::BLUE: return 'B';
        case Color::GREEN: return 'G';
        case Color::RED: return 'R';
        case Color::ORANGE: return 'O';
        default: return 'X'; // Internal/transparent face
    }
}

class CubeWASMAdapter: public IReport{
private:
    Cube* cube;

public:
    CubeWASMAdapter(Cube* cube): cube(cube){}

    std::string get_cube_state_JSON() const override{
        json resultJson;
        int size = cube->get_size();
        resultJson["size"] = size;

        json cubiesJson = json::array();

        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                for (int z = 0; z < size; z++) {
                    const Cubie* cubie = cube->get_cubie(x, y, z);
                    
                    json cubieJson;

                    // Position (using 0 to size-1 indices, as expected by frontend)
                    cubieJson["pos"] = {
                        {"x", x},
                        {"y", y},
                        {"z", z}
                    };

                    // Create a string of colors in 'R', 'L', 'U', 'D', 'F', 'B' order
                    std::string colors_string = "";
                    colors_string += color_to_char(cubie->get_color_from_face(FaceEnum::RIGHT));
                    colors_string += color_to_char(cubie->get_color_from_face(FaceEnum::LEFT));
                    colors_string += color_to_char(cubie->get_color_from_face(FaceEnum::UP));
                    colors_string += color_to_char(cubie->get_color_from_face(FaceEnum::DOWN));
                    colors_string += color_to_char(cubie->get_color_from_face(FaceEnum::FRONT));
                    colors_string += color_to_char(cubie->get_color_from_face(FaceEnum::BACK));

                    cubieJson["colors"] = colors_string;
                    cubiesJson.push_back(cubieJson);
                }
            }
        }
        resultJson["cubies"] = cubiesJson;
        return resultJson.dump();
    }
};