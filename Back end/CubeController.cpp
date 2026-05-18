#include "CubeController.hpp"
#include "Cube/FaceEnum.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_module)
{
    enum_<FaceEnum>("FaceEnum")
        .value("UP", FaceEnum::UP)
        .value("DOWN", FaceEnum::DOWN)
        .value("LEFT", FaceEnum::LEFT)
        .value("RIGHT", FaceEnum::RIGHT)
        .value("FRONT", FaceEnum::FRONT)
        .value("BACK", FaceEnum::BACK);

    class_<CubeController>("CubeController")
        .constructor<int>()
        .function("solve", &CubeController::get_solve_moves)
        .function("set_solver", &CubeController::set_solver)
        .function("apply_move", &CubeController::apply_move)
        .function("get_move_animation_info", &CubeController::get_move_animation_info)
        .function("scramble_cube", &CubeController::scramble_cube)
        .function("display_cube", &CubeController::display_cube)
        .function("set_cubie_color", &CubeController::set_cubie_color)
        .function("get_front_face", &CubeController::get_front_face)
        .function("get_up_face", &CubeController::get_up_face)
        .function("get_right_face", &CubeController::get_right_face)
        .function("get_left_face", &CubeController::get_left_face)
        .function("get_down_face", &CubeController::get_down_face)
        .function("get_back_face", &CubeController::get_back_face)
        .function("is_cube_solved", &CubeController::is_cube_solved)
        .function("is_cube_valid", &CubeController::is_cube_valid)
        .function("get_cube_state_JSON", &CubeController::get_cube_state)
        .function("is_cube_solvable", &CubeController::is_cube_solvable)
        .function("get_solution_JSON", &CubeController::get_solution_JSON);
}
#endif
