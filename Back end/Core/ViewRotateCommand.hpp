#pragma once
#include "ICommand.hpp"

class ViewRotateCommand:public ICommand{
private:
    Axis axis;
    int times;
public: 
    ViewRotateCommand(Axis axis, int times):axis(axis),times(times){}

    void execute(Cube* cube, Camera* camera) override{
        RotationMove move(axis, times);
        CameraRotator camera_rotator(camera);
        camera_rotator.rotate_camera(move);
    }
};