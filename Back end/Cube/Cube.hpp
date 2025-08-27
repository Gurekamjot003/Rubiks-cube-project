#pragma once
#include "FaceEnum.hpp"
#include "Cubie.hpp"
#include "../Cube Mechanics/Move.hpp"

class Cube{
    
private:
    bool is_solved;
    vector<vector<vector<Cubie*>>> cubies; 

    int n;

    pair<int, int> get_abs_coordinates(Axis axis, int i, int j){
        if(axis == Axis::X){
            return {n-1-i, n-1-j};
        }
        else if(axis == Axis::Y){
            return {j, i};
        }
        else if(axis == Axis::Z){
            return {n-1-j, i};
        }
        return {};
    }

    void swap_cubies(Axis axis, int layer, int r_from, int c_from, int r_to, int c_to){
        if(axis == Axis::X) swap(cubies[layer][r_from][c_from], cubies[layer][r_to][c_to]);
        else if(axis == Axis::Y) swap(cubies[r_from][layer][c_from], cubies[r_to][layer][c_to]);
        else if(axis == Axis::Z) swap(cubies[r_from][c_from][layer], cubies[r_to][c_to][layer]);
    }
    
    void d1_transpose(Axis axis, int layer){
        for(int i = 0; i<(n-1); i++){
            for(int j = i+1; j<n; j++){
                auto[r1,c1] = get_abs_coordinates(axis, i, j);
                auto[r2,c2] = get_abs_coordinates(axis, j, i);
                swap_cubies(axis, layer, r1, c1, r2, c2);
            }
        }
    }
    
    void d2_transpose(Axis axis, int layer){
        for(int i = 0; i<(n-1); i++){
            for(int j = (n-2-i); j>=0; j--){
                auto[r1,c1] = get_abs_coordinates(axis, i, j);
                auto[r2,c2] = get_abs_coordinates(axis, n-1-j, n-1-i);
                swap_cubies(axis, layer, r1, c1, r2, c2);
            }
        }
    }

    void row_reverse(Axis axis, int layer){
        for(int i = 0; i<n; i++){
            for(int j = 0; j<(n/2); j++){
                auto[r1,c1] = get_abs_coordinates(axis, i, j);
                auto[r2,c2] = get_abs_coordinates(axis, i, n-1-j);
                swap_cubies(axis, layer, r1, c1, r2, c2);
            }
        }
    }

    void column_reverse(Axis axis, int layer){
        for(int j = 0; j<n; j++){
            for(int i = 0; i<(n/2); i++){
                auto[r1,c1] = get_abs_coordinates(axis, i, j);
                auto[r2,c2] = get_abs_coordinates(axis, n-1-i, j);
                swap_cubies(axis, layer, r1, c1, r2, c2);
            }
        }
    }
public:
    Cube(int n):n(n){
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
    }

    Cubie* get_cubie(int x, int y, FaceEnum face);
        
    void set_color(FaceEnum face, int x, int y, Color color){
        
        Cubie* target_cubie = get_cubie(x, y, face);

        target_cubie->set_color(face, color);
    }

    void scramble_cube(){
        //scramble cube
    }

    vector<Cubie*> get_cubies_in_layer(Axis axis, int layer){
        vector<Cubie*> cubies_in_layer;
        int n = cubies.size();
        if(axis == Axis::X){
            for(int j=0; j<n; ++j){
                for(int k=0; k<n; ++k){
                    cubies_in_layer.push_back(cubies[layer][j][k]);
                }
            }
        } else if (axis == Axis::Y){
            for(int i=0; i<n; ++i){
                for(int k=0; k<n; ++k){
                    cubies_in_layer.push_back(cubies[i][layer][k]);
                }
            }
        } else if (axis == Axis::Z){
            for(int i=0; i<n; ++i){
                for(int j=0; j<n; ++j){
                    cubies_in_layer.push_back(cubies[i][j][layer]);
                }
            }
        }
        return cubies_in_layer;
    }

    vector<Cubie*> get_cubies_by_face(FaceEnum face){
        vector<Cubie*> ans;
        for(int i = 0; i<n; i++){
            for(int j = 0; j<n; j++){
                for(int k = 0; k<n; k++){
                    Cubie* cur = cubies[i][j][k];
                    if(cur->get_color(face) != Color::EMPTY){
                        ans.push_back(cur);
                    }
                }
            }
        }
        return ans;
    }
    
    void apply_move(Move move){
        
        Axis axis = move.get_axis();
        int layer = move.get_layer();
        int num_rotations = move.get_times();

        if(num_rotations == -1) num_rotations = 3;
        if(layer == 0) num_rotations = 4 - num_rotations;

        // cubies internal move
        vector<Cubie*> cubies_in_layer = get_cubies_in_layer(axis, layer);
        for(auto& cubie: cubies_in_layer) cubie->rotate_cubie(axis, layer, num_rotations);
        
        //apply move
        d1_transpose(axis, layer);

        if(num_rotations == 1) row_reverse(axis, layer);
        if(num_rotations == 2) d2_transpose(axis, layer);
        if(num_rotations == 3) column_reverse(axis, layer);
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