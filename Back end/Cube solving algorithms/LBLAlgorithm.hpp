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
        displayer = new CameraDisplayer(camera);

        moves.clear();
        // apply lbl algo

        // first we will find white center and put it in the down face and yellow will automatically be on the up face.
        white_down();

        // displayer.display(cube);

        // taking step by step solution reference from jperm
        // https://jperm.net/3x3
        // step 1 white cross
        white_cross();

        // step 2 first layer
        first_layer();

        // step 3 first layer corners

        // step 4 second layer edges

        // step 5 yellow cross

        // step 6 position yellow corners

        // step 7 orient yellow corners

        return moves;
    }

    bool test_daisy(){
        std::vector<Cubie*> up_edges = CubeGeometryUtils::get_cubies_by_face_and_type(camera->get_up_face(), cube, CubieType::EDGE);
        for(auto& cubie: up_edges){
            if(cubie->get_color_from_face(camera->get_up_face()) != Color::WHITE){
                return false;
            }
        }
        return true;
    }

    bool test_cross(){
        std::vector<Cubie*> down_edges = CubeGeometryUtils::get_cubies_by_face_and_type(camera->get_down_face(), cube, CubieType::EDGE);
        for(auto& cubie: down_edges){
            std::map<FaceEnum, Color> colors = cubie->get_colors();
            for(auto& [face, color]: colors){
                if(color != CameraUtils::get_face_color(camera, cube, face)){
                    return false;
                }
            }
        }
        return true;
    }

    void first_layer(){
        for(int temp = 0; temp<4; temp++){
            
            Color down_color = CameraUtils::get_face_color(camera, cube, camera->get_down_face()),
                  front_color = CameraUtils::get_face_color(camera, cube, camera->get_front_face()),
                  right_color = CameraUtils::get_face_color(camera, cube, camera->get_right_face());
                  
            std::vector<Color> colors_req = {down_color, front_color, right_color};
            Cubie* req_cubie = CubeGeometryUtils::get_cubies_by_colors(colors_req, cube)[0];
            

            // we have to place required corner piece to the front top right position if it is not on right & front face
                           
            if(req_cubie->check_faces_present({camera->get_down_face()})){
                // if the required cubie is present in down face, then we will apply move to bring it in up face
                int moves_applied = 0;
                while(!req_cubie->check_faces_present({camera->get_front_face(), camera->get_right_face()})){
                    apply_move("D");
                    moves_applied++;
                }
                ruru();
                // resetting the position
                while(moves_applied--){
                    apply_move("D'");
                }
            }
            else{
                // if the required cubie is present in up face, then we will apply move to bring it in correct position in first layer.
                while(!req_cubie->check_faces_present({camera->get_front_face(), camera->get_right_face()})){
                    apply_move("U");
                }
            }
            

            // now the required corner piece is in front top right position, we will apply move to place it in correct position in first layer.
            while(req_cubie->check_faces_present({camera->get_up_face()}) or req_cubie->get_color_from_face(camera->get_down_face()) != down_color){
                ruru();
            }
            apply_move("y");
        }
    }

    void white_cross()
    {
        // step 1 daisy
        daisy();
        if(!test_daisy()) std::cout<<"Error in daisy step\n";

        // step 2 white cross from daisy
        white_cross_from_daisy();
        if(!test_cross()) std::cout<<"Error in cross step\n";
        
    }


    void white_cross_from_daisy()
    {
        // find white edge pieces in daisy and move them to correct position in down face
        for (int temp = 0; temp < 4; temp++)
        {
            Cubie *up_front_cubie = CameraUtils::get_up_front_cubie(camera, cube);
            Color front_color = up_front_cubie->get_color_from_face(camera->get_front_face());
            while(front_color != CameraUtils::get_face_color(camera, cube, camera->get_front_face())){
                apply_move("d");
            }
            apply_move("F2");
            apply_move("y");
        }
            
    }

    void daisy()
    {
        // find white edge pieces and move them to up face around yellow center
        for (int temp = 0; temp < 4; temp++)
        { // for each edge
            std::vector<Cubie *> up = CubeGeometryUtils::get_cubies_by_face_and_type(camera->get_up_face(), cube, CubieType::EDGE);

            Cubie *up_front_cubie = CameraUtils::get_up_front_cubie(camera, cube);

            // case 1 - white edge piece is already in daisy
            if (up_front_cubie && up_front_cubie->get_color_from_face(camera->get_up_face()) == Color::WHITE)
            {
                apply_move("U");
                continue;
            }

            // case 2 - white edge piece is in down face, we will apply move to bring it in up face with white color facing front and then apply move to bring it in daisy.
            std::vector<Cubie *> down = CubeGeometryUtils::get_cubies_by_face_and_type(camera->get_down_face(), cube, CubieType::EDGE);
            Cubie *down_white_cubie = nullptr;

            for(auto& cubie: down){
                if(cubie->get_color_from_face(camera->get_down_face()) == Color::WHITE){
                    down_white_cubie = cubie;
                    break;
                }
            }

            while (down_white_cubie && !down_white_cubie->check_faces_present({camera->get_front_face()}))
            {
                apply_move("D");
            }

            if (down_white_cubie && down_white_cubie->get_color_from_face(camera->get_down_face()) == Color::WHITE)
            {
                apply_move("F2");
                apply_move("U");
                continue;
            }

            // case 3 - search for white edge piece in front face, until it is not found, the front face will be rotated
            Cubie *front_cubie = nullptr; // this is the cubie of concern, here we have to place white edge piece facing front

            while (!front_cubie)
            {
                std::vector<Cubie *> front = CubeGeometryUtils::get_cubies_by_face_and_type(camera->get_front_face(), cube, CubieType::EDGE);
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
            // displayer.display(cube);
        }
    }

    void white_down()
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