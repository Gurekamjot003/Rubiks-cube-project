#include <iostream>
#include <string>

#include "CubeController.hpp"

using namespace std;

int main() {
    cout << "\n\nWelcome to Gurekamjot's Rubik's Cube Solver! \n\n";
    cout << "Currently it is just a player and a work in progress\n";
    cout << "You can apply various moves on the cube all the moves are listed below \n\n";
    cout << "Normal moves - R L F B U D M E S\n";
    cout << "Rotation moves - x y z\n";
    cout << "Wide moves(multiple layers) - r/Rw, l/Lw, ...\n\n";
    cout << "Moves without suffix are clockwise moves and suffix ' are anticlockise (R') and 2 rotations are given by suffix 2\n";
    cout << "Prefix number gives layer number for normal move and layer count for wide move\n";
    cout << "Go to https://jperm.net/3x3/moves for more details\n\n";

    cout << "There are other functionalities inside CubeController class you can use them too to see how the cube works.\nEnjoy\n\n";

    CubeController c(3); // n*n sized cube will be formed

    c.display_cube();
    // string moves = c.get_solve_moves();
    // cout<<moves<<endl;
    // c.scramble_cube();

    // string moves = {"R U R' U x2 F2 U D D F2 U D F2 U d d F2 d' F U d F2 y d F2 y F2 y d F2 y x2"};
    string moves = {"y y' x2 y y' x2"};
    // string moves = {"m m' e e' s s' x2 m m' e e' s s' x2"};
    string cur;
    moves.push_back(' ');
    for(auto& move: moves)
    {
        if(move == ' ')
        {
            if(!cur.empty())
            {
                cout << "Applying move: " << cur << endl;
                c.apply_move(cur);
                c.display_cube();
            }
            cur.clear();
        }
        else
        {
            cur.push_back(move);
        }
    }

    c.apply_move("R U R' U");
    c.display_cube();
    c.apply_move("x2 F2 U D");
    c.display_cube();
    c.apply_move("D F2 U D");
    c.display_cube();
    c.apply_move("F2 U d d");
    c.display_cube();
    c.apply_move("F2 d' F U");
    c.display_cube();
    c.apply_move("d F2 y d");
    c.display_cube();
    c.apply_move("F2 y F2 y");
    c.display_cube();
    c.apply_move("d F2 y x2");
    c.display_cube();
    c.apply_move("y");
    c.display_cube();
    c.apply_move("y'");
    c.display_cube();

    c.apply_move("R U R' U"); // applying a move to test
    // c.apply_move("F R U R' U' F'"); // applying another move to test

    c.display_cube();

    // string moves = c.get_solve_moves();

    // c.display_cube();
    // moves.push_back(' ');
    // vector<string> move_list;
    // string cur;
    // for(auto &ch : moves)
    // {
    //     if(ch == ' ')
    //     {
    //         if(!cur.empty())
    //             move_list.push_back(cur);
    //         cur.clear();
    //     }
    //     else
    //     {
    //         cur.push_back(ch);
    //     }
    // }
    // for(auto& move: move_list)
    // {
    //     cout << "Applying move: " << move << endl;
    //     c.apply_move(move);
    //     c.display_cube();
    // }
    

    while (true) {
        cout << "\n\nEnter a move: ";
        string move;
        if (!std::getline(cin, move)) break; // handle EOF / input errors
        if (move == "-1") break;
        if (move.empty()) continue;

        c.apply_move(move);
        c.display_cube();
    }

    cout << "\n\nThanks for using Rubik's Cube Player.\n\n";
    cout << "Next up: Building the 3D visualizer with Three.js and implementing the solver.\n";
    cout << "Follow for updates!\n\n";

    return 0;
}