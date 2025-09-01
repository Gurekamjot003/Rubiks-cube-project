#include "CubeController.hpp"

int main(){
    CubeController c(3);

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