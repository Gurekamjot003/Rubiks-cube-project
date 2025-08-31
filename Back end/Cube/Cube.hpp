#pragma once
#include "FaceEnum.hpp"
#include "Cubie.hpp"
#include "../Cube Mechanics/Move.hpp"
#include "../Cube Mechanics/RotationMove.hpp"
#include "Coordinate.hpp"


class CubeRotator;

class Cube{
    
private:
    vector<vector<vector<Cubie*>>> cubies; 
    CubeRotator* rotator;
    int n;

    vector<Coordinate> corner_piece_coordinates;
public:
    Cube(int n):n(n),rotator(nullptr){
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
        map<set<Color>, int> corner_count, edge_count, middle_count;
        for(auto& i: cubies){
            for(auto& j: i){
                for(auto& cubie: j){
                    auto color_map = cubie->get_colors();
                    bool has_white = false, has_yellow = false, has_red = false, has_green = false, has_blue = false, has_orange = false;
                    set<Color> cur_colors;
                    for(auto&[face, color]: color_map){
                        if(cur_colors.count(color)) return false;
                        if(color == Color::WHITE) has_white = true;
                        else if(color == Color::YELLOW) has_yellow = true;
                        else if(color == Color::RED) has_red = true;
                        else if(color == Color::GREEN) has_green = true;
                        else if(color == Color::BLUE) has_blue = true;
                        else if(color == Color::ORANGE) has_orange = true;
                        cur_colors.insert(color);
                    }

                    if((has_white && has_yellow) or (has_red && has_orange) or (has_green && has_blue)) return false;

                    if(cubie->get_type() == CubieType::CORNER) corner_count[cur_colors]++;
                    else if(cubie->get_type() == CubieType::EDGE) edge_count[cur_colors]++;
                    else if(cubie->get_type() == CubieType::MIDDLE) middle_count[cur_colors]++;
                }
            }
        }

        // corner pieces count should be correct
        int req_corner_count = 1;
        if(corner_count.size() != 8) return false;
        for(auto&[st, freq]: corner_count){
            if(freq != req_corner_count) return false;
        }
        
        // edge pieces count should be correct
        int req_edges_count = max(0, n-2);
        if(edge_count.size() != 12) return false;
        for(auto&[st, freq]: edge_count){
            if(freq != req_edges_count) return false;
        }

        // check middle piece count
        int req_middle_count = max(0, (n-2)*(n-2));
        if(middle_count.size() != 6) return false;
        for(auto&[st, freq]: middle_count){
            if(freq != req_middle_count) return false;
        }   
        return true;
    }
    
    bool is_solved(){
        map<FaceEnum, Color> face_color;
        for(auto& i: cubies){
            for(auto& j: i){
                for(auto& cubie: j){
                    auto color_map = cubie->get_colors();
                    
                    for(auto&[face, color]: color_map){
                        if(face_color.count(face)){
                            if(face_color[face] != color) return false;
                        }
                        else{
                            face_color[face] = color;
                        }
                    }
                }
            }
        }
        return true;
        
    }

};

