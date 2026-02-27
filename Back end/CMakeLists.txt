cmake_minimum_required(VERSION 3.11)
project(RubiksCube)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --bind")

include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(RubiksCube 
    CubeController.cpp
    CubeController.hpp
    Core/FaceTurnCommand.hpp
    Core/ICommand.hpp
    Core/MoveParser.hpp
    Core/ViewRotateCommand.hpp
    Core/WideFaceTurnCommand.hpp
    Cube/Color.hpp
    Cube/Coordinate.hpp
    Cube/Cube.hpp
    Cube/CubieRotator.cpp
    Cube/CubeRotator.hpp
    Cube/Cubie.hpp
    Cube/CubieType.hpp
    Cube/FaceEnum.hpp
    "Cube Mechanics/Axis.hpp"
    "Cube Mechanics/Camera.hpp"
    "Cube Mechanics/CameraDisplayer.hpp"
    "Cube Mechanics/CubieColorSetter.hpp"
    "Cube Mechanics/Direction.hpp"
    "Cube Mechanics/Move.hpp"
    "Cube Mechanics/RotationMove.hpp"
    "Cube solving algorithms/CFOPAlgorithm.hpp"
    "Cube solving algorithms/CubeSolver.hpp"
    "Cube solving algorithms/CubeSolverFactory.hpp"
    "Cube solving algorithms/KoceimbaAlgorithm.hpp"
    "Cube solving algorithms/LBLAlgorithm.hpp"
    "Cube solving algorithms/SolverType.hpp"
    Utils/CameraUtils.hpp
    Utils/CubeGeometryUtils.cpp
    Utils/CubeGeometryUtils.hpp
    JSONAdapter/IReport.hpp
    JSONAdapter/CubeWASMAdapter.hpp

)

target_link_libraries(RubiksCube PRIVATE nlohmann_json::nlohmann_json)
