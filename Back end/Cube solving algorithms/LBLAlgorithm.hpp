#pragma once
#include "CubeSolver.hpp"

class LBLAlgorithm : public CubeSolver
{
    void ruru(int times = 1)
    {
        for (int i = 0; i < times; i++)
        {
            apply_move("R U R' U'");
        }
    }

    void lulu(int times = 1)
    {
        for (int i = 0; i < times; i++)
        {
            apply_move("L' U' L U");
        }
    }

public:
    LBLAlgorithm(Cube *c)
    {
        this->cube = c;
    }
    std::vector<std::string> solve(Cube *cube, Camera *camera) override
    {
        this->cube = cube;
        this->camera = camera;
        CameraDisplayer displayer(camera);

        moves.clear();
        // apply lbl algo

        // first we will find white center and put it in the down face and yellow will automatically be on the up face.
        white_down(displayer);

        displayer.display(cube);
        // step 1 white cross
        white_cross(displayer);

        // step 2 cross

        // step 3 first layer corners

        // step 4 second layer edges

        // step 5 yellow cross

        // step 6 position yellow corners

        // step 7 orient yellow corners

        return moves;
    }

    void white_cross(CameraDisplayer &displayer)
    {
        // step 1 daisy
        daisy(displayer);

        // step 2 white cross from daisy

    }

    

    void white_cross_from_daisy(CameraDisplayer &displayer)
    {
        // find white edge pieces in daisy and move them to correct position in down face
        for (int temp = 0; temp < 4; temp++)
        {
            Cubie *up_front_cubie = CameraUtils::get_up_front_cubie(camera, cube);
            Color front_color = up_front_cubie->get_color_from_face(camera->get_front_face());
            for(int t = 0; t<4; t++){

            }
        }
            
    }

    void daisy(CameraDisplayer &displayer)
    {
        // find white edge pieces and move them to up face around yellow center
        for (int temp = 0; temp < 4; temp++)
        { // for each edge
            std::vector<Cubie *> up = CubeGeometryUtils::get_cubie_pieces_by_face(camera->get_up_face(), cube, CubieType::EDGE);

            Cubie *up_front_cubie = CameraUtils::get_up_front_cubie(camera, cube);

            // case 1 - white edge piece is already in daisy
            if (up_front_cubie && up_front_cubie->get_color_from_face(camera->get_up_face()) == Color::WHITE)
            {
                apply_move("U");
                displayer.display(cube);
                continue;
            }

            // case 2 - white edge piece is in down face, we will apply move to bring it in up face with white color facing front and then apply move to bring it in daisy.
            std::vector<Cubie *> down = CubeGeometryUtils::get_cubie_pieces_by_face(camera->get_down_face(), cube, CubieType::EDGE);
            Cubie *down_front_cubie = CameraUtils::get_down_front_cubie(camera, cube);

            while (down_front_cubie && !down_front_cubie->check_faces_present({camera->get_front_face()}))
            {
                apply_move("D");
                displayer.display(cube);
            }

            if (down_front_cubie && down_front_cubie->get_color_from_face(camera->get_down_face()) == Color::WHITE)
            {
                apply_move("F2");
                apply_move("U");
                displayer.display(cube);
                continue;
            }

            // case 3 - search for white edge piece in front face, until it is not found, the front face will be rotated
            Cubie *front_cubie = nullptr; // this is the cubie of concern, here we have to place white edge piece facing front

            while (!front_cubie)
            {
                std::vector<Cubie *> front = CubeGeometryUtils::get_cubie_pieces_by_face(camera->get_front_face(), cube, CubieType::EDGE);
                for (auto &cubie : front)
                {
                    if (cubie->get_color_from_face(camera->get_front_face()) == Color::WHITE)
                    {
                        front_cubie = cubie;
                        break;
                    }
                }
                if (!front_cubie)
                {
                    apply_move("d");
                    displayer.display(cube);    
                }
            }

            // now we are having white edge piece in front face, we want to just see the position on which it is present and apply move accordingly to place it in daisy.
            // place the white cubie on right face then the further moves will be common
            if (front_cubie->check_faces_present({camera->get_left_face()}))
            {
                apply_move("F2");
            }
            else if (front_cubie->check_faces_present({camera->get_down_face()}))
            {
                apply_move("F'");
            }
            else if (front_cubie->check_faces_present({camera->get_up_face()}))
            {
                apply_move("F");
            }

            // common move from right to up face
            apply_move("d' F");

            // next white edge piece
            apply_move("U");
            displayer.display(cube);
        }
    }

    void white_down(CameraDisplayer &displayer)
    {
        // find white center
        if(CameraUtils::get_face_color(camera, cube, camera->get_up_face()) == Color::WHITE){
            apply_move("x2");
            return;
        }
        if(CameraUtils::get_face_color(camera, cube, camera->get_front_face()) == Color::WHITE){
            apply_move("x'");
            return;
        }
        if(CameraUtils::get_face_color(camera, cube, camera->get_back_face()) == Color::WHITE){
            apply_move("x");
            return;
        }
        if(CameraUtils::get_face_color(camera, cube, camera->get_left_face()) == Color::WHITE){
            apply_move("z'");
            return;
        }
        if(CameraUtils::get_face_color(camera, cube, camera->get_right_face()) == Color::WHITE){
            apply_move("z");
            return;
        }
    }
};