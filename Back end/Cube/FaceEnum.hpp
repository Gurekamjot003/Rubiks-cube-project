#pragma once
#include<bits/stdc++.h>
using namespace std;

enum class FaceEnum{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

FaceEnum get_opposite_face(FaceEnum face){
    if(face == FaceEnum::FRONT) return FaceEnum::BACK;        
    if(face == FaceEnum::BACK) return FaceEnum::FRONT;
    if(face == FaceEnum::UP) return FaceEnum::DOWN;
    if(face == FaceEnum::DOWN) return FaceEnum::UP;
    if(face == FaceEnum::RIGHT) return FaceEnum::LEFT;
    if(face == FaceEnum::LEFT) return FaceEnum::RIGHT;
    return FaceEnum::FRONT; // Should not happen
    
}