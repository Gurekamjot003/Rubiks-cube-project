#pragma once
#include "FaceEnum.hpp"
#include "Cubie.hpp"
#include "../Cube Mechanics/Move.hpp"
#include "../Cube Mechanics/RotationMove.hpp"
#include "Coordinate.hpp"


class CubeRotator;

class Cube{
    
private:
    bool is_solved;
    vector<vector<vector<Cubie*>>> cubies; 
    CubeRotator* rotator;
    int n;

    vector<Coordinate> corner_piece_coordinates;
public:
    Cube(int n):n(n),rotator(nullptr){
        is_solved = true;
        // Initialize cubies
        cubies.resize(n, vector<vector<Cubie*>>(n, vector<Cubie*>(n, nullptr)));
        for(int i=0; i<n; ++i){
            for(int j=0; j<n; ++j){
                for(int k=0; k<n; ++k){
                    map<FaceEnum, Color> cubie_colors;
                    if(i == 0) cubie_colors[FaceEnum::LEFT] = Color::ORANGE;
                    if(i == n-1) cubie_colors[FaceEnum::RIGHT] = Color::RED;
                    if(j == 0) cubie_colors[FaceEnum::DOWN] = Color::YELLOW;
                    if(j == n-1) cubie_colors[FaceEnum::UP] = Color::WHITE;
                    if(k == 0) cubie_colors[FaceEnum::BACK] = Color::BLUE;
                    if(k == n-1) cubie_colors[FaceEnum::FRONT] = Color::GREEN;
                    cubies[i][j][k] = new Cubie(cubie_colors);
                }
            }
        }
        corner_piece_coordinates = {Coordinate(0, 0), Coordinate(0, n-1), Coordinate(n-1, 0), Coordinate(n-1, n-1)};
    }

    void swap_cubies(Axis axis, int layer, Coordinate from, Coordinate to){
        int r_from = from.get_x();
        int c_from = from.get_y();
        int r_to = to.get_x();
        int c_to = to.get_y();
        if(axis == Axis::X) swap(cubies[layer][r_from][c_from], cubies[layer][r_to][c_to]);
        else if(axis == Axis::Y) swap(cubies[r_from][layer][c_from], cubies[r_to][layer][c_to]);
        else if(axis == Axis::Z) swap(cubies[r_from][c_from][layer], cubies[r_to][c_to][layer]);
    }

    Cubie* get_cubie(Axis axis, int layer, Coordinate coordinate){
        int i = coordinate.get_x(), j = coordinate.get_y();

        if(axis == Axis::X) return cubies[layer][i][j];
        else if(axis == Axis::Y) return cubies[i][layer][j];
        else if(axis == Axis::Z) return cubies[i][j][layer];
        return nullptr; // Should not happen
    }
        
    void set_color_by_abs_coordinates(FaceEnum face, Axis axis, int layer, Coordinate coordinate, Color color){
        Cubie* target_cubie = get_cubie(axis, layer, coordinate);
        target_cubie->set_color_by_face(face, color);
    }

    void scramble_cube(){
        //scramble cube
    }

    vector<vector<Cubie*>> get_cubies_in_layer(Axis axis, int layer, Coordinate top_left = Coordinate(0, 0), Coordinate top_right = Coordinate(0, 1)){
        int di = 1, end_i = n;
        int dj = 1, end_j = n;
        int start_i = top_left.get_x(), start_j = top_left.get_y();
        if(start_i != 0){
            end_i = -1;
            di = -1;
        }
        if(start_j != 0){
            end_j = -1;
            dj = -1;
        }
        
        vector<vector<Cubie*>> cubies_in_layer;
        if(top_left.get_x() == top_right.get_x()){ // loop x befor y
            for(int i = start_i; i != end_i; i+=di){
                vector<Cubie*> cur_row;
                for(int j = start_j; j!=end_j; j+=dj){
                    if(axis == Axis::X) cur_row.push_back(cubies[layer][i][j]);
                    else if(axis == Axis::Y) cur_row.push_back(cubies[i][layer][j]);
                    else if(axis == Axis::Z) cur_row.push_back(cubies[i][j][layer]);
                }
                cubies_in_layer.push_back(cur_row);
            }
        }
        else{ // loop y before x
            for(int j = start_j; j!=end_j; j+=dj){
                vector<Cubie*> cur_row;
                for(int i = start_i; i != end_i; i+=di){
                    if(axis == Axis::X) cur_row.push_back(cubies[layer][i][j]);
                    else if(axis == Axis::Y) cur_row.push_back(cubies[i][layer][j]);
                    else if(axis == Axis::Z) cur_row.push_back(cubies[i][j][layer]);
                }
                cubies_in_layer.push_back(cur_row);
            }   
        }
        
        return cubies_in_layer;
    }

    Coordinate get_corner_piece_coordinate_with_required_faces(Axis axis, int layer, vector<FaceEnum> required_faces){

        for(auto&coor: corner_piece_coordinates){
            int i = coor.get_x();
            int j = coor.get_y();
            Cubie* cubie_to_search;
            if(axis == Axis::X) cubie_to_search = cubies[layer][i][j];
            else if(axis == Axis::Y) cubie_to_search = cubies[i][layer][j];
            else if(axis == Axis::Z) cubie_to_search = cubies[i][j][layer];

            if(cubie_to_search->check_faces_present(required_faces)) return coor;
        }
        return Coordinate();
    }
    
    int get_size(){
        return n;
    }

    bool is_cube_valid(){
        // color freq should be n*n 

        // will apply these later
        // edge pieces count should be correct
        

        // corner pieces count should be correct
        return true;
    }

};

