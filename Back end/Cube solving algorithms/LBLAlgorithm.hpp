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

        // // step 2 first layer
        // first_layer();

        // // step 3 second layer
        // second_layer();

        // // step 4 top cross
        // top_cross();

        // // step 5 corner positioning
        // corner_positioning();

        // // step 6 corner twisting
        // corner_twisting();

        // // step 7 edge positioning - solved cube
        // edge_positioning();

        return moves;
    }

    // int correct_top_edges_count(){
    //     std::vector<Cubie*> up_face_cubies = CubeGeometryUtils::get_cubies_by_face_and_type(camera->get_up_face(), cube, CubieType::EDGE);
    //     int count = 0;
    //     for(auto& cubie: up_face_cubies){
    //         if(CubeGeometryUtils::check_cubie_position(cubie, cube)){
    //             count++;
    //         }
    //     }
    //     return count;
    // }

    void edge_positioning_move_sequence(){
        ruru();
        lulu();
        ruru(5);
        lulu(5);
    }

    // void edge_positioning(){
        
    //     while(correct_top_edges_count() != 4){
    //         if(correct_top_edges_count() == 0){
    //             edge_positioning_move_sequence();
    //             continue;
    //         }
    //         Cubie* front_top = CubeGeometryUtils::get_cubies_by_faces({camera->get_front_face(), camera->get_up_face()}, cube)[0];
    //         while(!CubeGeometryUtils::check_cubie_position(front_top, cube)){
    //             apply_move("y");
    //             front_top = CubeGeometryUtils::get_cubies_by_faces({camera->get_front_face(), camera->get_up_face()}, cube)[0];
    //         }
    //         edge_positioning_move_sequence();
    //     }
    // }

    // void corner_twisting(){
    //     // we have to hold the cube with white face up and fix each yellow corner one by one
    //     apply_move("x2");

    //     Color down_color = CubeGeometryUtils::get_face_color(cube, camera->get_down_face());
    //     for(int side = 0; side<4; side++){
    //         Cubie* bottom_right = CubeGeometryUtils::get_cubies_by_faces({camera->get_front_face(), camera->get_right_face(), camera->get_down_face()}, cube)[0];   
    //         while(bottom_right->get_color_from_face(camera->get_down_face()) != down_color){
    //             ruru();
    //         }
    //         apply_move("D");
    //     }

    //     // make the cube upright again
    //     apply_move("x2");
    // }

    // std::vector<Cubie*> correct_top_corners(){
    //     std::vector<Cubie*> up_face_cubies = CubeGeometryUtils::get_cubies_by_face_and_type(camera->get_up_face(), cube, CubieType::CORNER);
    //     std::vector<Cubie*> correct_corners;
    //     for(auto& cubie: up_face_cubies){
    //         std::vector<Color> colors_req;
    //         for(auto&[face, color]: cubie->get_colors()){
    //             colors_req.push_back(CubeGeometryUtils::get_face_color(cube, face));
    //         }
    //         if(cubie->check_colors_match(colors_req)){
    //             correct_corners.push_back(cubie);
    //         }
    //     }
    //     return correct_corners;
    // }

    // void corner_positioning_move_sequence(){
    //     ruru(3);
    //     apply_move("y");
    //     lulu(3);
    //     apply_move("U'");
    // }

    // void corner_positioning(){
    //     while(correct_top_corners().size() != 4){
    //         if(correct_top_corners().size() != 2){
    //             apply_move("U");
    //             continue;
    //         }
    //         // now we have 2 correct corners, we will apply move to make them at right and then apply move to position the other 2 corners correctly
    //         std::vector<Cubie*> correct_corners = correct_top_corners();
            
    //         bool found_left = false;
    //         for(int i = 0; i<4; i++){
    //             if(!(correct_corners[0]->check_faces_present({camera->get_left_face()}) && correct_corners[1]->check_faces_present({camera->get_left_face()}))){
    //                 apply_move("y");
    //             }
    //             else{
    //                 found_left = true;
    //                 corner_positioning_move_sequence();
    //                 break;
    //             }
    //         }

    //         // if we don't find left, it is case of alternate correct
    //         if(!found_left){
    //             // we will apply move to make them at right and then apply move to position the other 2 corners correctly
    //             corner_positioning_move_sequence();
    //         }
    //     }
    // }
            

    // bool test_top_cross(){

    //     Color up_color = CubeGeometryUtils::get_face_color(cube, camera->get_up_face());
    //     std::vector<Cubie*> up_face_cubies = CubeGeometryUtils::get_cubies_by_face_and_type(camera->get_up_face(), cube, CubieType::EDGE);
    //     for(auto& cubie: up_face_cubies){
    //         if(cubie->get_color_from_face(camera->get_up_face()) != up_color) return false;
    //     }
    //     return true;
    // }

    // void top_cross(){

    //     Color up_color = CubeGeometryUtils::get_face_color(cube, camera->get_up_face());
    //     std::vector<Cubie*> up_face_cubies = CubeGeometryUtils::get_cubies_by_face_and_type(camera->get_up_face(), cube, CubieType::EDGE);

    //     while(!test_top_cross()){
    //         // we want any of the 3 cases - reference to jperm step 4
            
    //         // case 1 only single middle piece all edge pieces incorrect

    //         // case 2 2 pieces correct
    //         std::vector<Cubie*> cubies_with_correct_edge_pieces;
    //         for(auto& cubie: up_face_cubies){
    //             if(cubie->get_color_from_face(camera->get_up_face()) == up_color){
    //                 cubies_with_correct_edge_pieces.push_back(cubie);
    //             }
    //         }

    //         if(cubies_with_correct_edge_pieces.size() == 1 or cubies_with_correct_edge_pieces.size() == 3){
    //             throw std::runtime_error("Error in top cross algorithm, there should be either 0 or 2 edge pieces with correct up face color. The cube is unsolvable or there is some error in previous steps.");
    //         }
    //         // we have to make top face such that correct are at left and back or left and right
    //         if(cubies_with_correct_edge_pieces.size() == 2){
    //             // if the correct pieces are at left and back or left and right, then we will apply move to make them at left and back and then apply move to solve the cross
    //             while(true){
    //                 if(cubies_with_correct_edge_pieces[1]->check_faces_present({camera->get_left_face()})){
    //                     std::swap(cubies_with_correct_edge_pieces[0], cubies_with_correct_edge_pieces[1]);
    //                 }
                        
    //                 if(cubies_with_correct_edge_pieces[0]->check_faces_present({camera->get_left_face()}) and (cubies_with_correct_edge_pieces[1]->check_faces_present({camera->get_right_face()})) or (cubies_with_correct_edge_pieces[1]->check_faces_present({camera->get_back_face()}))){
    //                     break;
    //                 }
                    
    //                 apply_move("U");
    //             }
                
    //         }

    //         apply_move("F R U R' U' F'");
    //     }
    // }

    
    // void move_sequence_for_top_to_right_move(){
    //     apply_move("U");
    //     ruru();
    //     apply_move("y");
    //     lulu();
    //     apply_move("y'");
    //     apply_move("U'");
    // }

    // void move_sequence_for_top_to_left_move(){
    //     apply_move("U'");
    //     lulu();
    //     apply_move("y'");
    //     ruru();
    //     apply_move("y");
    //     apply_move("U");
    // }

    // void second_layer(){
    //     for(int side = 0; side<4; side++){
    //         Color front_color = CubeGeometryUtils::get_face_color(cube, camera->get_front_face());
    //         Color right_color = CubeGeometryUtils::get_face_color(cube, camera->get_right_face());
    //         Cubie* req_cubie = CubeGeometryUtils::get_cubies_by_colors({front_color, right_color}, cube)[0];


    //         //case 1 - if the required edge piece is already in second layer in its correct position
    //         std::map<FaceEnum, Color> face_color_map = {
    //             {camera->get_front_face(), front_color},
    //             {camera->get_right_face(), right_color}
    //         };

    //         if(req_cubie->check_faces_match(face_color_map)){
    //             apply_move("y");
    //             continue;
    //         }

    //         // case 2 - if the required edge piece is in second layer but in wrong position, then we will apply move to bring it in up face
    //         if(!req_cubie->check_faces_present({camera->get_up_face()})){
    //             int move_count = 0;
    //             while(!req_cubie->check_faces_present({camera->get_front_face(), camera->get_right_face()})){
    //                 apply_move("d");
    //                 move_count++;
    //             }
    //             move_sequence_for_top_to_right_move();
    //             while(move_count--){
    //                 apply_move("d'");
    //             }
    //         }

    //         // now cubie is in top layer we want it on front face of top layer
    //         while(!req_cubie->check_faces_present({camera->get_front_face()})){
    //             apply_move("U");
    //         }

    //         // now cubie is in front on top layer it has to be moved to right, there can be 2 cases for the position of the cubie.

    //         // case 1 - if front color matches with front face color, then we will apply move to bring it in right face
    //         if(req_cubie->get_color_from_face(camera->get_front_face()) == front_color){
    //             move_sequence_for_top_to_right_move();
    //         }
    //         // case 2 - if right color matches with front face color, then we will apply move to bring it in left face
    //         else{
    //             apply_move("U' y");
    //             move_sequence_for_top_to_left_move();
    //             apply_move("y' U");
    //         }


    //         apply_move("y");
    //     }
    // }

    // void first_layer(){
    //     for(int temp = 0; temp<4; temp++){
            
    //         Color down_color = CubeGeometryUtils::get_face_color(cube, camera->get_down_face()),
    //               front_color = CubeGeometryUtils::get_face_color(cube, camera->get_front_face()),
    //               right_color = CubeGeometryUtils::get_face_color(cube, camera->get_right_face());
                  
    //         std::vector<Color> colors_req = {down_color, front_color, right_color};
    //         Cubie* req_cubie = CubeGeometryUtils::get_cubies_by_colors(colors_req, cube)[0];
            

    //         // we have to place required corner piece to the front top right position if it is not on right & front face
                           
    //         if(req_cubie->check_faces_present({camera->get_down_face()})){
    //             // if the required cubie is present in down face, then we will apply move to bring it in up face
    //             int moves_applied = 0;
    //             while(!req_cubie->check_faces_present({camera->get_front_face(), camera->get_right_face()})){
    //                 apply_move("D");
    //                 moves_applied++;
    //             }
    //             ruru();
    //             // resetting the position
    //             while(moves_applied--){
    //                 apply_move("D'");
    //             }
    //         }
    //         else{
    //             // if the required cubie is present in up face, then we will apply move to bring it in correct position in first layer.
    //             while(!req_cubie->check_faces_present({camera->get_front_face(), camera->get_right_face()})){
    //                 apply_move("U");
    //             }
    //         }
            

    //         // now the required corner piece is in front top right position, we will apply move to place it in correct position in first layer.
    //         while(req_cubie->check_faces_present({camera->get_up_face()}) or req_cubie->get_color_from_face(camera->get_down_face()) != down_color){
    //             ruru();
    //         }
    //         apply_move("y");
    //     }
    // }

    void white_cross()
    {
        // step 1 daisy
        daisy();
        // if(!test_daisy()) std::cout<<"Error in daisy step\n";

        // step 2 white cross from daisy
        white_cross_from_daisy();
        // if(!test_cross()) std::cout<<"Error in cross step\n";
        
    }

    void white_cross_from_daisy()
    {
        // find white edge pieces in daisy and move them to correct position in down face
        for (int side = 0; side < 4; side++)
        {
            Cubie *up_front_cubie = CubeGeometryUtils::get_cubies_by_faces_matching({camera->get_up_face(), camera->get_front_face()}, cube)[0];
            Color front_color = up_front_cubie->get_color_from_face(camera->get_front_face());
            while(front_color != CubeGeometryUtils::get_face_color(cube, camera->get_front_face())){
                apply_move("d");
            }
            apply_move("F2");
            apply_move("y");
        }
            
    }

    void daisy()
    {   
        // find white edge pieces, there can be maximum 4 such pieces
        std::vector<Cubie*> white_edge_cubies = CubeGeometryUtils::get_cubies_by_colors_present({Color::WHITE}, cube, CubieType::EDGE);
        // move them to up face around yellow center
        for (auto& cubie: white_edge_cubies)
        { // for each edge

            // case 1 - white edge piece is already in up face, move to next cubie
            if(cubie->check_faces_present({camera->get_up_face()}) && cubie->get_color_from_face(camera->get_up_face()) == Color::WHITE){
                continue;
            }

            // case 2 - white edge piece is in down face, we will apply move to bring it in up face and then apply move to bring it in daisy.
            if(cubie->check_faces_present({camera->get_down_face()}) && cubie->get_color_from_face(camera->get_down_face()) == Color::WHITE){
                move_white_edge_from_down_to_daisy(cubie);
                continue;
            }
           

            // now we are having white edge piece with white color neither facing in up face nor in down face
            // case 3 - search for white edge piece in front face, until it is not found, the front face will be rotated
            while(cubie && !(cubie->check_faces_present({camera->get_front_face()}) && cubie->get_color_from_face(camera->get_front_face()) == Color::WHITE)){
                apply_move("y");
            }

            // we want to just see the position on which it is present and apply move accordingly to place it in daisy.
            make_up_space_for_daisy();
            // place the white cubie on right face then the further moves will be common
            if (cubie->check_faces_present({camera->get_left_face()}))
            {
                apply_move("F2");
            }
            else if (cubie->check_faces_present({camera->get_down_face()}))
            {
                apply_move("F'");
            }
            else if (cubie->check_faces_present({camera->get_up_face()}))
            {
                apply_move("F");
            }

            // common move from right to up face
            apply_move("d' F");

            // displayer.display(cube);
        }
    }

    void move_white_edge_from_down_to_daisy(Cubie* cubie){
        // we will apply move to bring it in up face with white color facing front and then apply move to bring it in daisy.
        while(cubie && !cubie->check_faces_present({camera->get_front_face()})){
            apply_move("D");
        }

        make_up_space_for_daisy();
        
        apply_move("F2");
    }

    void make_up_space_for_daisy(){
        Cubie* up_front_cubie = CubeGeometryUtils::get_cubies_by_faces_matching({camera->get_front_face(), camera->get_up_face()}, cube)[0];
        while(up_front_cubie->get_color_from_face(camera->get_up_face()) == Color::WHITE){
            apply_move("U");
            up_front_cubie = CubeGeometryUtils::get_cubies_by_faces_matching({camera->get_front_face(), camera->get_up_face()}, cube)[0];
        }
    }

    void white_down()
    {
        // find white center
        Cubie* white_center = CubeGeometryUtils::get_cubies_by_colors_matching({Color::WHITE}, cube)[0];

        if(white_center->check_faces_match({camera->get_up_face()})){
            apply_move("x2");
            return;
        }
        if(white_center->check_faces_match({camera->get_front_face()})){
            apply_move("x'");
            return;
        }
        if(white_center->check_faces_match({camera->get_back_face()})){
            apply_move("x");
            return;
        }
        if(white_center->check_faces_match({camera->get_left_face()})){
            apply_move("z'");
            return;
        }
        if(white_center->check_faces_match({camera->get_right_face()})){
            apply_move("z");
            return;
        }
    }

    // bool test_daisy(){
    //     std::vector<Cubie*> up_edges = CubeGeometryUtils::get_cubies_by_face_and_type(camera->get_up_face(), cube, CubieType::EDGE);
    //     for(auto& cubie: up_edges){
    //         if(cubie->get_color_from_face(camera->get_up_face()) != Color::WHITE){
    //             return false;
    //         }
    //     }
    //     return true;
    // }

    // bool test_cross(){
    //     std::vector<Cubie*> down_edges = CubeGeometryUtils::get_cubies_by_face_and_type(camera->get_down_face(), cube, CubieType::EDGE);
    //     for(auto& cubie: down_edges){
    //         std::map<FaceEnum, Color> colors = cubie->get_colors();
    //         for(auto& [face, color]: colors){
    //             if(color != CubeGeometryUtils::get_face_color(cube, face)){
    //                 return false;
    //             }
    //         }
    //     }
    //     return true;
    // }


};