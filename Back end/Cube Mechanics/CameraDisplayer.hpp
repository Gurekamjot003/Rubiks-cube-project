#pragma once
#include "../Utils/CameraUtils.hpp"
#include <iostream>
#include <string> // Add this include

// Define color codes for readability
const std::string RESET_COLOR = "\033[0m";
const std::string RED_COLOR = "\e[1;31m";
const std::string GREEN_COLOR = "\e[1;32m";
const std::string YELLOW_COLOR = "\e[1;33m";
const std::string BLUE_COLOR = "\e[1;34m";
const std::string WHITE_COLOR = "\e[1;37m";
const std::string ORANGE_COLOR = "\e[1;38;5;208m"; 

class CameraDisplayer{
    Camera* camera;

    std::string get_color_string(Color color) {
        char color_char = 'X';
        std::string color_code = "";

        if (color == Color::BLUE) { color_char = 'B'; color_code = BLUE_COLOR; }
        if (color == Color::GREEN) { color_char = 'G'; color_code = GREEN_COLOR; }
        if (color == Color::RED) { color_char = 'R'; color_code = RED_COLOR; }
        if (color == Color::ORANGE) { color_char = 'O'; color_code = ORANGE_COLOR; }
        if (color == Color::WHITE) { color_char = 'W'; color_code = WHITE_COLOR; }
        if (color == Color::YELLOW) { color_char = 'Y'; color_code = YELLOW_COLOR; }
        
        // Return the color code, the character, and the reset code
        return color_code + color_char + RESET_COLOR;
    }
public:
    CameraDisplayer(Camera* camera): camera(camera){}

    std::vector<std::vector<Color>> face_displayer(FaceEnum face_to_display, Cube* cube){
        Coordinate top_left = CameraUtils::get_top_left_coordinate_by_face_and_camera(face_to_display, camera, cube);
        Coordinate top_right = CameraUtils::get_top_right_coordinate_by_face_and_camera(face_to_display, camera, cube);
        std::vector<std::vector<Cubie*>> cubies = CubeGeometryUtils::get_cubies_by_face(face_to_display, cube, top_left, top_right);
        std::vector<std::vector<Color>> face_colors;
        for(auto& cubie_row: cubies){
            std::vector<Color> cur_row;
            for(auto& cubie: cubie_row){
                Color color = cubie->get_color_from_face(face_to_display);
                cur_row.push_back(color);
            }
            face_colors.push_back(cur_row);
        }
        return face_colors;
    }

    void display(Cube* cube) {
        FaceEnum front_face = camera->get_front_face();
        FaceEnum up_face = camera->get_up_face();
        FaceEnum right_face = camera->get_right_face();

        std::cout << "\n\n--------Current cube state---------\n\n";

        std::vector<std::vector<Color>> up_colors = face_displayer(up_face, cube);
        for (auto& row : up_colors) {
            for (auto& color : row) {
                std::cout << get_color_string(color) << " "; // Use the new function
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        
        std::vector<std::vector<Color>> front_colors = face_displayer(front_face, cube);
        std::vector<std::vector<Color>> right_colors = face_displayer(right_face, cube);
        
        for (int i = 0; i < cube->get_size(); i++) {
            for (auto& color : front_colors[i]) std::cout << get_color_string(color) << " ";
            std::cout << "  ";
            for (auto& color : right_colors[i]) std::cout << get_color_string(color) << " ";
            std::cout<<std::endl;
        }
        std::cout << std::endl;

    }
};