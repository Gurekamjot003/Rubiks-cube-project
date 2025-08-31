#include "CubeController.hpp"

int main(){
    CubeController c(3);

    c.display_cube();

    c.apply_move("x");

    c.set_cubie_color(c.get_front_face(), 0, 0, Color::RED);
    c.display_cube();
    c.set_cubie_color(c.get_front_face(), 1, 0, Color::RED);
    c.display_cube();
    c.set_cubie_color(c.get_right_face(), 1, 0, Color::GREEN);
    c.display_cube();
    c.set_cubie_color(c.get_up_face(), 1, 0, Color::YELLOW);
    c.display_cube();
    while(true){
        cout<<"\n\nEnter a move: ";
        string move; 
        getline(cin, move);
        if(move == "-1") break;
        
        c.apply_move(move);

        
        c.display_cube();
    }
    
}