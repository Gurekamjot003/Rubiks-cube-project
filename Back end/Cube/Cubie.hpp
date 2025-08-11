#pragma once
#include "Color.hpp"

class Cubie{
private:
    Color color;
public:
    Cubie(Color color){
        this->color = color;
    }
    
    Color get_color(){
        return color;
    }
    
    void set_color(Color color){
        this->color = color;
    }
};