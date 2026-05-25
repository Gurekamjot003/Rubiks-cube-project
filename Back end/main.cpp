#include <iostream>
#include <string>

#include "CubeController.hpp"

using namespace std;

void apply_moves_one_by_one(CubeController &controller, const string &moves)
{
    string cur_move;
    for (char ch : moves)
    {
        if (ch == ' ')
        {
            if (!cur_move.empty())
            {
                cout << "Applying move: " << cur_move << endl;
                controller.apply_move(cur_move);
                controller.display_cube();
            }
            cur_move.clear();
        }
        else
        {
            cur_move.push_back(ch);
        }
    }
}

int main()
{
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
    c.set_cubie_color(FaceEnum::UP, 0, 0, Color::RED);
    c.display_cube();
    c.scramble_cube();
    c.display_cube();
    string moves = c.get_solve_moves();

    apply_moves_one_by_one(c, moves);
    c.display_cube();
    

    while (true)
    {
        cout << "\n\nEnter a move: ";
        string move;
        if (!std::getline(cin, move))
            break; // handle EOF / input errors
        if (move == "-1")
            break;
        if (move.empty())
            continue;

        c.apply_move(move);
        c.display_cube();
    }

    cout << "\n\nThanks for using Rubik's Cube Player.\n\n";
    cout << "Next up: Building the 3D visualizer with Three.js and implementing the solver.\n";
    cout << "Follow for updates!\n\n";

    return 0;
}