#pragma once
#include "Cubie.hpp"

class Face{
private:    
    vector<vector<Cubie>> face;
public:
    Face(vector<vector<Cubie>> face){
        this->face = face;
    }

    Face(Color c, int n){
        face.resize(n, vector<Cubie>(n, Cubie(c)));
    }
    
    vector<vector<Cubie>> get_face(){
        return face;
    }
    
    void set_face(vector<vector<Cubie>> face){
        this->face = face;
    }

    void set_color(int x, int y, Color c){
        face[x][y].set_color(c);
    }

    Color get_color(int x, int y){
        return face[x][y].get_color();
    }

    int get_length(){
        return face.size();
    }

    int get_width(){
        return face[0].size();       
    }
};