#pragma once
#include "CubeSolver.hpp"

class LBLAlgorithm: public CubeSolver{
    void ruru(){
        apply_move("R U R' U'");
    }

    void lulu(){
        apply_move("L' U' L U");
    }

    public:
    LBLAlgorithm(){}
    std::vector<std::string> solve(Cube* cube, Camera* camera) override{
        this->cube = cube;
        this->camera = camera;
        moves.clear();
        // apply lbl algo
        
        // first we will find white center and put it in the down face and yellow will automatically be on the up face. 
        white_down();

        // step 1 white cross
        white_cross();

        // step 2 cross

        // step 3 first layer corners

        // step 4 second layer edges

        // step 5 yellow cross

        // step 6 position yellow corners

        // step 7 orient yellow corners


        return moves;
    }

    void white_cross(){
        // step 1 daisy
        for(int temp = 0; temp<4; temp++){ // for each edge
            std::vector<Cubie*> up = CubeGeometryUtils::get_cubie_pieces_by_face(camera->get_up_face(), cube, CubieType::EDGE);
            
            Cubie* up_front_cubie = nullptr; // this is the cubie of concern, here we have to place white edge piece facing up
            for(auto& cubie: up){
                if(!cubie->check_faces_present({camera->get_up_face(), camera->get_front_face()})) continue;
                up_front_cubie = cubie;
                break;
            }

            // case 1 - white edge piece is already in daisy
            if(up_front_cubie and up_front_cubie->get_color_from_face(camera->get_up_face()) == Color::WHITE){
                apply_move("U");
                continue;
            }

            // case 2 - white edge piece is in down face, we will apply move to bring it in up face with white color facing front and then apply move to bring it in daisy.
            std::vector<Cubie*> down = CubeGeometryUtils::get_cubie_pieces_by_face(camera->get_down_face(), cube, CubieType::EDGE);
            Cubie* down_front_cubie = nullptr; // this is the cubie of concern, here we have to place white edge piece facing down
            for(auto& cubie: down){
                if(cubie->get_color_from_face(camera->get_down_face()) != Color::WHITE) continue;
                down_front_cubie = cubie;
                break;
            }

            while(down_front_cubie and !down_front_cubie->check_faces_present({camera->get_front_face()})){
                apply_move("D");
            }

            if(down_front_cubie and down_front_cubie->get_color_from_face(camera->get_down_face()) == Color::WHITE){
                apply_move("F2");
                apply_move("U");
                continue;
            }

            // case 3 - search for white edge piece in front face, until it is not found, the front face will be rotated
            std::vector<Cubie*> front = CubeGeometryUtils::get_cubie_pieces_by_face(camera->get_front_face(), cube, CubieType::EDGE);
            Cubie* front_cubie = nullptr; // this is the cubie of concern, here we have to place white edge piece facing front
            
            while(!front_cubie){
                for(auto& cubie: front){
                    if(cubie->get_color_from_face(camera->get_front_face()) == Color::WHITE){
                        front_cubie = cubie;
                        break;
                    }
                }
                if(!front_cubie){
                    apply_move("d");
                }
            }

            // now we are having white edge piece in front face, we want to just see the position on which it is present and apply move accordingly to place it in daisy.
            // place the white cubie on right face then the further moves will be common
            if(front_cubie->check_faces_present({camera->get_left_face()})){
                apply_move("F2");
            }
            else if(front_cubie->check_faces_present({camera->get_down_face()})){
                apply_move("F'");
            }
            else if(front_cubie->check_faces_present({camera->get_up_face()})){
                apply_move("F");
            }

            // common move from right to up face
            apply_move("d' F");

            // next white edge piece
            apply_move("U");
        }

        // step 2 white cross from daisy
    }

    void white_down(){
        // find white center

        std::vector<Cubie*> down = CubeGeometryUtils::get_cubie_pieces_by_face(FaceEnum::DOWN, cube, CubieType::MIDDLE);
        if(down[0]->get_color_from_face(camera->get_down_face()) == Color::WHITE) return;

        std::vector<Cubie*> up = CubeGeometryUtils::get_cubie_pieces_by_face(FaceEnum::UP, cube, CubieType::MIDDLE);
        if(up[0]->get_color_from_face(camera->get_up_face()) == Color::WHITE){
            apply_move("x2");
            return;
        }

        std::vector<Cubie*> front = CubeGeometryUtils::get_cubie_pieces_by_face(FaceEnum::FRONT, cube, CubieType::MIDDLE);
        if(front[0]->get_color_from_face(camera->get_front_face()) == Color::WHITE){
            apply_move("x'");
            return;
        }

        std::vector<Cubie*> back = CubeGeometryUtils::get_cubie_pieces_by_face(FaceEnum::BACK, cube, CubieType::MIDDLE);
        if(back[0]->get_color_from_face(camera->get_back_face()) == Color::WHITE){
            apply_move("x");
            return;
        }

        std::vector<Cubie*> left = CubeGeometryUtils::get_cubie_pieces_by_face(FaceEnum::LEFT, cube, CubieType::MIDDLE);
        if(left[0]->get_color_from_face(camera->get_left_face()) == Color::WHITE){
            apply_move("z'");
            return;
        }

        std::vector<Cubie*> right = CubeGeometryUtils::get_cubie_pieces_by_face(FaceEnum::RIGHT, cube, CubieType::MIDDLE);
        if(right[0]->get_color_from_face(camera->get_right_face()) == Color::WHITE){
            apply_move("z");
            return;
        }
    }
};