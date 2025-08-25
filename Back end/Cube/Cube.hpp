#pragma once
#include "FaceEnum.hpp"
#include "Cubie.hpp"
#include "../Cube Mechanics/Move.hpp"

class Cube{
    
private:
    bool is_solved;
    vector<vector<vector<Cubie*>>> cubies; 

    int n;
    
    void d1_transpose(Axis axis, int layer){
        if(axis == Axis::X)
        for(int i = 0; i<(n-1); i++){
            for(int j = i+1; j<n; j++){
                swap(cubies[layer][i][j], cubies[layer][j][i]);
            }
        }
        else if(axis == Axis::Y)
        for(int i = 0; i<(n-1); i++){
            for(int j = i+1; j<n; j++){
                swap(cubies[i][layer][j], cubies[j][layer][i]);
            }
        }
        else if(axis == Axis::Z)
        for(int i = 0; i<(n-1); i++){
            for(int j = i+1; j<n; j++){
                swap(cubies[i][j][layer], cubies[j][i][layer]);
            }
        }
    }

    void d2_transpose(Axis axis, int layer){
        if(axis == Axis::X)
        for(int i = 0; i<(n-1); i++){
            for(int j = (n-2-i); j>=0; j--){
                swap(cubies[layer][i][j], cubies[layer][j][i]);
            }
        }
        else if(axis == Axis::Y)
        for(int i = 0; i<(n-1); i++){
            for(int j = (n-2-i); j>=0; j--){
                swap(cubies[i][layer][j], cubies[j][layer][i]);
            }
        }
        else if(axis == Axis::Z)
        for(int i = 0; i<(n-1); i++){
            for(int j = (n-2-i); j>=0; j--){
                swap(cubies[i][j][layer], cubies[j][i][layer]);
            }
        }
    }

    void row_reverse(Axis axis, int layer){
        if(axis == Axis::X)
        for(int i = 0; i<n; i++){
            for(int j = 0; j<(n/2); j++){
                swap(cubies[layer][i][j], cubies[layer][i][n-1-j]);
            }
        }
        else if(axis == Axis::Y)
        for(int i = 0; i<n; i++){
            for(int j = 0; j<(n/2); j++){
                swap(cubies[i][layer][j], cubies[i][layer][n-1-j]);
            }
        }
        else if(axis == Axis::Z)
        for(int i = 0; i<n; i++){
            for(int j = 0; j<(n/2); j++){
                swap(cubies[i][j][layer], cubies[i][n-1-j][layer]);
            }
        }
    }

    void column_reverse(Axis axis, int layer){
        if(axis == Axis::X){
            for(int j = 0; j<n; j++){
                for(int i = 0; i<(n/2); i++){
                    swap(cubies[layer][i][j], cubies[layer][n-1-i][j]);
                }
            }
        }
        else if(axis == Axis::Y){
            for(int k = 0; k<n; k++){
                for(int i = 0; i<(n/2); i++){
                    swap(cubies[i][layer][k], cubies[n-1-i][layer][k]);
                }
            }
        }
        else if(axis == Axis::Z){
            for(int j = 0; j<n; j++){
                for(int i = 0; i<(n/2); i++){
                    swap(cubies[i][j][layer], cubies[n-1-i][j][layer]);
                }
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