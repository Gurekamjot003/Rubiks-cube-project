#pragma once
#include "../Utils/CubeGeometryUtils.hpp"

class CubeRotator{
    
    Cube* cube;
    int n;
    void d1_transpose(Axis axis, int layer){
        for(int i = 0; i<(n-1); i++){
            for(int j = i+1; j<n; j++){
                auto[r1,c1] = CubeGeometryUtils::get_abs_coordinates(axis, i, j, cube);
                auto[r2,c2] = CubeGeometryUtils::get_abs_coordinates(axis, j, i, cube);
                cube->swap_cubies(axis, layer, r1, c1, r2, c2);
            }
        }
    }
    
    void d2_transpose(Axis axis, int layer){
        for(int i = 0; i<(n-1); i++){
            for(int j = (n-2-i); j>=0; j--){
                auto[r1,c1] = CubeGeometryUtils::get_abs_coordinates(axis, i, j, cube);
                auto[r2,c2] = CubeGeometryUtils::get_abs_coordinates(axis, n-1-j, n-1-i, cube);
                cube->swap_cubies(axis, layer, r1, c1, r2, c2);
            }
        }
    }

    void row_reverse(Axis axis, int layer){
        for(int i = 0; i<n; i++){
            for(int j = 0; j<(n/2); j++){
                auto[r1,c1] = CubeGeometryUtils::get_abs_coordinates(axis, i, j, cube);
                auto[r2,c2] = CubeGeometryUtils::get_abs_coordinates(axis, i, n-1-j, cube);
                cube->swap_cubies(axis, layer, r1, c1, r2, c2);
            }
        }
    }

    void column_reverse(Axis axis, int layer){
        for(int j = 0; j<n; j++){
            for(int i = 0; i<(n/2); i++){
                auto[r1,c1] = CubeGeometryUtils::get_abs_coordinates(axis, i, j, cube);
                auto[r2,c2] = CubeGeometryUtils::get_abs_coordinates(axis, n-1-i, j, cube);
                cube->swap_cubies(axis, layer, r1, c1, r2, c2);
            }
        }
    }
public:
    CubeRotator(Cube* cube){
        this->cube = cube;
        n = cube->get_size();
    }

    void rotate_cube(Move& move){
        Axis axis = move.get_axis();
        int layer = move.get_layer();
        int num_rotations = move.get_times();

        // cubies internal move
        vector<Cubie*> cubies_in_layer = cube->get_cubies_in_layer(axis, layer);
        for(auto& cubie: cubies_in_layer){
            RotationMove rotation_move(axis, num_rotations);
            cubie->rotate_cubie(rotation_move);
        }
        
        //apply move
        d1_transpose(axis, layer);

        if(num_rotations == 1) row_reverse(axis, layer);
        if(num_rotations == 2) d2_transpose(axis, layer);
        if(num_rotations == 3) column_reverse(axis, layer);
    }

};


void Cube::apply_move(Move move){
    if(!rotator) rotator = new CubeRotator(this);
    rotator->rotate_cube(move);
}