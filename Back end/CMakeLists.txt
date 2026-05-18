cmake_minimum_required(VERSION 3.17)
project(RubiksCube)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --bind -sALLOW_MEMORY_GROWTH")
endif()

# Create a minimal nlohmann_json interface library (header-only, no fetching)
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(RUBIKS_CUBE_SOURCES
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

add_executable(RubiksCube ${RUBIKS_CUBE_SOURCES})
target_link_libraries(RubiksCube PRIVATE nlohmann_json)

if(NOT EMSCRIPTEN)
    add_executable(RubiksCubeConsole main.cpp ${RUBIKS_CUBE_SOURCES})
    target_link_libraries(RubiksCubeConsole PRIVATE nlohmann_json)
endif()
