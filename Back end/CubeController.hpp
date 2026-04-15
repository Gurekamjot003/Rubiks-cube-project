#pragma once
#include "Core/MoveParser.hpp"
#include "Cube solving algorithms/CubeSolverFactory.hpp"
#include "Cube Mechanics/CameraDisplayer.hpp"
#include "Cube Mechanics/CubieColorSetter.hpp"
#include "JSONAdapter/CubeWASMAdapter.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class CubeController
{
private:
    std::unique_ptr<Cube> cube;
    std::unique_ptr<CubeSolver> solver;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<CameraDisplayer> displayer;
    std::unique_ptr<IReport> reporter;

public:
    CubeController(int n)
    {
        cube = std::make_unique<Cube>(n);
        solver = CubeSolverFactory::get_solver(SolverType::LBL, cube.get());
        camera = std::make_unique<Camera>();
        displayer = std::make_unique<CameraDisplayer>(camera.get());
        reporter = std::make_unique<CubeWASMAdapter>(cube.get());
    }

    void solve()
    {
        solver->solve();
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

    void scramble_cube()
    {
        cube->scramble_cube();
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
            // view rotates ignored for now
        }
        return arr.dump();
    }

    bool is_cube_solved()
    {
        return cube->is_solved();
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

    std::string get_cube_state()
    {
        return reporter->get_cube_state_JSON();
    }
};