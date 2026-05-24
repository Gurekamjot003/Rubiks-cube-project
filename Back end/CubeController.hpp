#pragma once
#include "Core/MoveParser.hpp"
#include "Cube solving algorithms/CubeSolverFactory.hpp"
#include "Cube Mechanics/CameraDisplayer.hpp"
#include "Cube Mechanics/CubieColorSetter.hpp"
#ifdef EMSCRIPTEN
#include "JSONAdapter/CubeWASMAdapter.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#endif
#include <random>
#include <chrono>

class CubeController
{
private:
    std::unique_ptr<Cube> cube;
    std::unique_ptr<CubeSolver> solver;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<CameraDisplayer> displayer;
#ifdef EMSCRIPTEN
    std::unique_ptr<IReport> reporter;
#endif

public:
    CubeController(int n)
    {
        cube = std::make_unique<Cube>(n);
        solver = CubeSolverFactory::get_solver(SolverType::LBL, cube.get());
        camera = std::make_unique<Camera>();
        displayer = std::make_unique<CameraDisplayer>(camera.get());
#ifdef EMSCRIPTEN
        reporter = std::make_unique<CubeWASMAdapter>(cube.get());
#endif
    }

    std::string get_solve_moves()
    {
        if (!solver)
            set_solver(SolverType::LBL); // Default to LBL if no solver is set

        Cube cube_copy(*cube);       // Create a copy of the cube to solve
        Camera camera_copy(*camera); // Create a copy of the camera for the solver

        // Solve the copied cube and get the moves
        std::vector<std::string> moves = solver->solve(&cube_copy, &camera_copy);

        // Join the moves into a single string separated by spaces
        std::string result;
        for (const auto &move : moves)
        {
            result += move + " ";
        }

        return result;
    }

    void set_solver(SolverType type)
    {
        solver = CubeSolverFactory::get_solver(type, cube.get());
    }

    void apply_move(std::string move)
    {
        std::vector<std::unique_ptr<ICommand>> commands = MoveParser::parse(move, camera.get(), cube.get());
        for (auto &command : commands)
        {
            command->execute(cube.get(), camera.get());
        }
    }

    void scramble_cube(int count = 20)
    {
        // apply some random moves to scramble the cube
        std::vector<std::string> scramble_moves = {"R", "U", "R'", "U'", "L'", "U'", "L", "U"};
        for (int i = 0; i < count; ++i)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, scramble_moves.size() - 1);
            apply_move(scramble_moves[dis(gen)]);
        }
    }

    void display_cube()
    {
        displayer->display(cube.get());
    }

    void set_cubie_color(FaceEnum face, int i, int j, Color color)
    {
        // top left of current face is assumed to be 0, 0
        Coordinate target(i, j);
        std::unique_ptr<CubieColorSetter> setter = std::make_unique<CubieColorSetter>(camera.get(), cube.get());
        setter->set_color(face, target, color);
    }

    const FaceEnum get_front_face()
    {
        return camera->get_front_face();
    }

    const FaceEnum get_up_face()
    {
        return camera->get_up_face();
    }

    const FaceEnum get_right_face()
    {
        return camera->get_right_face();
    }

    const FaceEnum get_left_face()
    {
        return camera->get_left_face();
    }

    const FaceEnum get_down_face()
    {
        return camera->get_down_face();
    }

    const FaceEnum get_back_face()
    {
        return camera->get_back_face();
    }

    // return animation info for a given move (without modifying cube state)
#ifdef EMSCRIPTEN
    std::string get_move_animation_info(std::string move)
    {
        auto commands = MoveParser::parse(move, camera.get(), cube.get());
        json arr = json::array();
        for (auto &cmd : commands)
        {
            if (auto f = dynamic_cast<FaceTurnCommand *>(cmd.get()))
            {
                json obj;
                obj["type"] = "face";
                obj["axis"] = (f->getAxis() == Axis::X ? "X" : (f->getAxis() == Axis::Y ? "Y" : "Z"));
                obj["layer"] = f->getLayer();
                obj["rotations"] = f->getTimes();
                arr.push_back(obj);
            }
            else if (auto w = dynamic_cast<WideFaceTurnCommand *>(cmd.get()))
            {
                json obj;
                obj["type"] = "wide";
                obj["axis"] = (w->getAxis() == Axis::X ? "X" : (w->getAxis() == Axis::Y ? "Y" : "Z"));
                obj["layer"] = w->getLayer();
                obj["rotations"] = w->getTimes();
                obj["num_layers"] = w->getNumLayers();
                // this wide move should animate all layers at once
                obj["simultaneous"] = true;
                arr.push_back(obj);
            }
        }
        return arr.dump();
    }
#endif

    bool is_cube_solved()
    {
        return CubeGeometryUtils::check_cube_solved(cube.get());
    }

    bool is_cube_valid()
    {
        return cube->is_cube_valid();
    }

    bool is_cube_solvable()
    {
        if (!cube->is_cube_valid())
            return false;
        // check for if cube is solvable or not
        // will apply this later
        return true;
    }

    // return JSON string representation of cube state
#ifdef EMSCRIPTEN
    std::string get_cube_state()
    {
        return reporter->get_cube_state_JSON();
    }
#endif

    // returns the solution moves as a JSON string with timing information
#ifdef EMSCRIPTEN
    std::string get_solution_JSON(std::string solverType = "LBL")
    {
        if (!solver)
            set_solver(SolverType::LBL);

        Cube cube_copy(*cube);
        Camera camera_copy(*camera);

        // Measure the time taken to solve the cube
        auto start_time = std::chrono::high_resolution_clock::now();
        std::vector<std::string> moves = solver->solve(&cube_copy, &camera_copy);
        auto end_time = std::chrono::high_resolution_clock::now();

        // Calculate elapsed time in milliseconds
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        double elapsed_ms = duration.count();

        json solution = json::object();
        solution["moves"] = moves;
        solution["moveCount"] = moves.size();
        solution["solveTimeMs"] = elapsed_ms;  // Time taken to find solution (in milliseconds)

        return solution.dump();
    }
#endif
};