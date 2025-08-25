#include "CubeController.hpp"

int main(){
    CubeController c(3);

    c.display_cube();
    c.apply_move("r");
    c.display_cube();
    c.apply_move("u");
    c.display_cube();

}