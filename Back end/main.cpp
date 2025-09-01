#include "CubeController.hpp"
using namespace std;

int main(){
    cout<<"\n\nWelcome to Gurekamjot's Rubik's Cube Solver! \n\n";
    cout<<"Currently it is just a player ans a work in progress\n";
    cout<<"You can apply various moves on the cube all the moves are listed below \n\n";
    cout<<"Normal moves - R L F B U D M E S\n";
    cout<<"Rotation moves - x y z\n";
    cout<<"Wide moves(multiple layers) - r/Rw, l/Lw, ...\n\n";
    cout<<"Moves without suffix are clockwise moves and suffix \' are anticlockise (R\') and 2 rotations are given by suffix 2\n";
    cout<<"Prefix number gives layer number for normal move and layer count for wide move\n";
    cout<<"Go to https://jperm.net/3x3/moves for more details\n\n";

    cout<<"There are other functionalities inside CubeController class you can use them too to see how the cube works.\nEnjoy\n\n";
    
    CubeController c(3); // n*n sized cube will be formed

    c.display_cube();

    while(true){
        cout<<"\n\nEnter a move: ";
        string move; 
        getline(cin, move); // Try using "R U R' U'"
        if(move == "-1") break;
        
        c.apply_move(move);

        
        c.display_cube();
    }
    
}