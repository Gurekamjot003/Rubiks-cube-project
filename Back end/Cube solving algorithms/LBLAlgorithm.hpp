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
        first_layer();

        // // step 3 second layer
        second_layer();

        // // step 4 top cross
        top_cross();

        // // step 5 corner positioning
        corner_positioning();

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

    void edge_positioning_move_sequence()
    {
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

    void corner_positioning_move_sequence(){
        ruru(3);
        apply_move("y");
        lulu(3);
        apply_move("U'");
    }

    bool alternate_correct_corner_positioning(){
        std::vector<std::vector<Cubie*>> cubies = CameraUtils::get_cubies_in_display_order(camera->get_up_face(), camera, cube);
        std::vector<std::vector<std::vector<int>>> correct_corner_patterns = {
            {// for alternate correct
                {0,2,1},
                {2,1,2},
                {1,2,0}
            },
            {// for alternate correct
                {1,2,0},
                {2,1,2},
                {0,2,1}  // 1 for correct position, 0 for incorrect position and 2 for don't care
            }
        };

        for(auto& pattern: correct_corner_patterns){
            bool match = true;
            for(int i = 0; i<3; i++){
                for(int j = 0; j<3; j++){
                    if(pattern[i][j] != 2 and (pattern[i][j] != CubeGeometryUtils::check_cubie_position(cubies[i][j], cube))){
                        match = false;
                        break;
                    }
                }
                if(!match) break;
            }
            if(match) return true;
        }
        return false;
    }

    bool incorrect_corner_positioning(){
        std::vector<std::vector<Cubie*>> cubies = CameraUtils::get_cubies_in_display_order(camera->get_up_face(), camera, cube);
        std::vector<std::vector<int>> pattern = {
            {1,2,0},
            {2,1,2},
            {1,2,0}     // 1 for correct position, 0 for incorrect position and 2 for don't care
        };
        
        for(int i = 0; i<3; i++){
            for(int j = 0; j<3; j++){
                if(pattern[i][j] != 2 and (pattern[i][j] != CubeGeometryUtils::check_cubie_position(cubies[i][j], cube))){
                    return false;
                }
            }
        }        
        return true;
    }

    int correct_top_corners_count(){
        std::vector<Cubie*> up_face_cubies = CubeGeometryUtils::get_cubies_by_faces_present({camera->get_up_face()}, cube, CubieType::CORNER);
        int count = 0;
        for(auto& cubie: up_face_cubies){
            if(CubeGeometryUtils::check_cubie_position(cubie, cube)){
                count++;
            }
        }
        return count;
    }

    void corner_positioning(){

        while(correct_top_corners_count() != 4){
            if(correct_top_corners_count() != 2){
                apply_move("U");
                continue;
            }
            if(alternate_correct_corner_positioning()){
                corner_positioning_move_sequence();
            }
            else if(incorrect_corner_positioning()){
                corner_positioning_move_sequence();
            }
            else{
                apply_move("y");
            }
        }
    }

    bool test_top_cross(){

        Color up_color = CubeGeometryUtils::get_face_color(cube, camera->get_up_face());
        std::vector<Cubie*> up_face_cubies = CubeGeometryUtils::get_cubies_by_colors_present({up_color}, cube, CubieType::EDGE);
        for(auto& cubie: up_face_cubies){
            if(cubie->get_color_from_face(camera->get_up_face()) != up_color) return false;
        }
        return true;
    }

    bool check_top_orientation_for_yellow_cross(){
        std::vector<std::vector<std::vector<int>>> up_face_color_pattern = {
            {
                {2,0,2},
                {0,1,0},
                {2,0,2}
            },     // for single middle piece correct
            {
                {2,1,2},
                {1,1,0},
                {2,0,2} // for back and left
            },
            {
                {2,0,2}, // for left and right
                {1,1,1},
                {2,0,2}
            }
        };
        std::vector<std::vector<Cubie*>> up_cubies = CameraUtils::get_cubies_in_display_order(camera->get_up_face(), camera, cube);
        Color up_color = CubeGeometryUtils::get_face_color(cube, camera->get_up_face());
        for(auto& pattern: up_face_color_pattern){
            bool match = true;
            for(int i = 0; i<3; i++){
                for(int j = 0; j<3; j++){
                    if(pattern[i][j] != 2 and (pattern[i][j] != (up_cubies[i][j]->get_color_from_face(camera->get_up_face()) == up_color))){
                        match = false;
                        break;
                    }
                }
            }
            if(match) return true;
        }
        return false;
    }

    void top_cross(){

        while(!test_top_cross()){
            // we want any of the 3 cases - reference to jperm step 4

            while(!check_top_orientation_for_yellow_cross()){
                apply_move("U");
            }

            apply_move("F R U R' U' F'");
        }
    }

    void move_sequence_for_top_to_right_move(){
        apply_move("U");
        ruru();
        apply_move("y");
        lulu();
        apply_move("y'");
        apply_move("U'");
    }

    void move_sequence_for_top_to_left_move(){
        apply_move("U'");
        lulu();
        apply_move("y'");
        ruru();
        apply_move("y");
        apply_move("U");
    }

    void second_layer(){
        std::vector<Cubie*> edge_cubies_2nd_layer = CubeGeometryUtils::get_cubies_by_colors_not_present({Color::WHITE, Color::YELLOW}, cube);
        for(auto& cubie: edge_cubies_2nd_layer){

            if(CubeGeometryUtils::check_cubie_position_strict(cubie, cube)){
                continue;
            }
            
            // bring cubie to to front and right if present in mid layer
            if(!cubie->check_faces_present({camera->get_up_face()})){
                while(!cubie->check_faces_present({camera->get_front_face(), camera->get_right_face()})){
                    apply_move("y");
                }
                move_sequence_for_top_to_right_move();
            }

            // bring the cubie to the front face
            while(!cubie->check_faces_present({camera->get_front_face()})){
                apply_move("y");
            }

            // now match center color with cubie
            while(cubie->get_color_from_face(camera->get_front_face()) != CubeGeometryUtils::get_face_color(cube, camera->get_front_face())){
                apply_move("d");
            }

            // if cubie matches with right face center color, then we will apply move to bring it in right face, else we will apply move to bring it in left face
            if(cubie->get_color_from_face(camera->get_up_face()) == CubeGeometryUtils::get_face_color(cube, camera->get_right_face())){
                move_sequence_for_top_to_right_move();
            }
            else{
                move_sequence_for_top_to_left_move();
            }
        }
    }

    void first_layer()
    {

        std::vector<Cubie *> white_corner_cubies = CubeGeometryUtils::get_cubies_by_colors_present({Color::WHITE}, cube, CubieType::CORNER);
        for (auto &cubie : white_corner_cubies)
        {

            // if the white corner piece is already in correct position in correct orientation, move to next cubie
            if (CubeGeometryUtils::check_cubie_position_strict(cubie, cube))
            {
                continue;
            }
            
            // get cubie to front right part of cube
            while (!cubie->check_faces_present({camera->get_front_face(), camera->get_right_face()}))
            {
                apply_move("y");
            }

            // if cubie is at down face, we will apply move to bring it in up face
            if(cubie->check_faces_present({camera->get_down_face()})){
                ruru();
            }

            // cubie is in up face with incorrect position & orientation
            // we will check by putting cubie in right bottom from up face one by one on every face to check correct position
            while (true)
            {
                std::vector<Color> colors_required = {
                    CubeGeometryUtils::get_face_color(cube, camera->get_front_face()),
                    CubeGeometryUtils::get_face_color(cube, camera->get_right_face()),
                    CubeGeometryUtils::get_face_color(cube, camera->get_down_face())
                };
                if (cubie->check_colors_match(colors_required))
                {
                    ruru();
                    break;
                }
                apply_move("d");
            }

            // case 4 - now cubie is in bottom right corner in correct position but we have to correct orientation if required
            while (!CubeGeometryUtils::check_cubie_position_strict(cubie, cube))
            {
                ruru();
            }
        }
    }

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
            while (front_color != CubeGeometryUtils::get_face_color(cube, camera->get_front_face()))
            {
                apply_move("d");
            }
            apply_move("F2");
            apply_move("y");
        }
    }

    void daisy()
    {
        // find white edge pieces, there can be maximum 4 such pieces
        std::vector<Cubie *> white_edge_cubies = CubeGeometryUtils::get_cubies_by_colors_present({Color::WHITE}, cube, CubieType::EDGE);
        // move them to up face around yellow center
        for (auto &cubie : white_edge_cubies)
        { // for each edge

            // case 1 - white edge piece is already in up face, move to next cubie
            if (cubie->check_faces_present({camera->get_up_face()}) && cubie->get_color_from_face(camera->get_up_face()) == Color::WHITE)
            {
                continue;
            }

            // case 2 - white edge piece is in down face, we will apply move to bring it in up face and then apply move to bring it in daisy.
            if (cubie->check_faces_present({camera->get_down_face()}) && cubie->get_color_from_face(camera->get_down_face()) == Color::WHITE)
            {
                move_white_edge_from_down_to_daisy(cubie);
                continue;
            }

            // now we are having white edge piece with white color neither facing in up face nor in down face
            // case 3 - search for white edge piece in front face, until it is not found, the front face will be rotated
            while (cubie && !(cubie->check_faces_present({camera->get_front_face()}) && cubie->get_color_from_face(camera->get_front_face()) == Color::WHITE))
            {
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

    void move_white_edge_from_down_to_daisy(Cubie *cubie)
    {
        // we will apply move to bring it in up face with white color facing front and then apply move to bring it in daisy.
        while (cubie && !cubie->check_faces_present({camera->get_front_face()}))
        {
            apply_move("D");
        }

        make_up_space_for_daisy();

        apply_move("F2");
    }

    void make_up_space_for_daisy()
    {
        Cubie *up_front_cubie = CubeGeometryUtils::get_cubies_by_faces_matching({camera->get_front_face(), camera->get_up_face()}, cube)[0];
        while (up_front_cubie->get_color_from_face(camera->get_up_face()) == Color::WHITE)
        {
            apply_move("U");
            up_front_cubie = CubeGeometryUtils::get_cubies_by_faces_matching({camera->get_front_face(), camera->get_up_face()}, cube)[0];
        }
    }

    void white_down()
    {
        // find white center
        Cubie *white_center = CubeGeometryUtils::get_cubies_by_colors_matching({Color::WHITE}, cube)[0];

        if (white_center->check_faces_match({camera->get_up_face()}))
        {
            apply_move("x2");
            return;
        }
        if (white_center->check_faces_match({camera->get_front_face()}))
        {
            apply_move("x'");
            return;
        }
        if (white_center->check_faces_match({camera->get_back_face()}))
        {
            apply_move("x");
            return;
        }
        if (white_center->check_faces_match({camera->get_left_face()}))
        {
            apply_move("z'");
            return;
        }
        if (white_center->check_faces_match({camera->get_right_face()}))
        {
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