#pragma once
#include "../Utils/CubeGeometryUtils.hpp"

class CubeRotator{
    
    Cube* cube;
    int n;
    void d1_transpose(Axis axis, int layer){
        for(int i = 0; i<(n-1); i++){
            for(int j = i+1; j<n; j++){
                Coordinate from = CubeGeometryUtils::get_abs_coordinates(axis, Coordinate(i, j), cube);
                Coordinate to = CubeGeometryUtils::get_abs_coordinates(axis, Coordinate(j, i), cube);
                cube->swap_cubies(axis, layer, from, to);
            }
        }
    }
    
    void d2_transpose(Axis axis, int layer){
        for(int i = 0; i<(n-1); i++){
            for(int j = (n-2-i); j>=0; j--){
                Coordinate from = CubeGeometryUtils::get_abs_coordinates(axis, Coordinate(i, j), cube);
                Coordinate to = CubeGeometryUtils::get_abs_coordinates(axis, Coordinate(n-1-j, n-1-i), cube);
                cube->swap_cubies(axis, layer, from, to);
            }
        }
    }

    void row_reverse(Axis axis, int layer){
        for(int i = 0; i<n; i++){
            for(int j = 0; j<(n/2); j++){
                Coordinate from = CubeGeometryUtils::get_abs_coordinates(axis, Coordinate(i, j), cube);
                Coordinate to = CubeGeometryUtils::get_abs_coordinates(axis, Coordinate(i, n-1-j), cube);
                cube->swap_cubies(axis, layer, from, to);
            }
        }
    }

    void column_reverse(Axis axis, int layer){
        for(int j = 0; j<n; j++){
            for(int i = 0; i<(n/2); i++){
                Coordinate from = CubeGeometryUtils::get_abs_coordinates(axis, Coordinate(i, j), cube);
                Coordinate to = CubeGeometryUtils::get_abs_coordinates(axis, Coordinate(n-1-i, j), cube);
                cube->swap_cubies(axis, layer, from, to);
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
        vector<vector<Cubie*>> cubies_in_layer = cube->get_cubies_in_layer(axis, layer);
        RotationMove rotation_move(axis, num_rotations);
        for(auto& cubie_row: cubies_in_layer){
            for(auto& cubie: cubie_row) cubie->rotate_cubie(rotation_move);
        }
        
        //apply move
        d1_transpose(axis, layer);

        if(num_rotations == 1) row_reverse(axis, layer);
        if(num_rotations == 2) d2_transpose(axis, layer);
        if(num_rotations == 3) column_reverse(axis, layer);
    }

};