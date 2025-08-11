# C++ Rubik's Cube Solver & 3D Visualizer

A high-performance 3x3x3 Rubik's Cube solver built with a robust C++ back end, demonstrating advanced object-oriented design and a dynamic 3D web-based visualizer.

**[Live Demo Link Here]**



## Core Features

* **Interactive 3D Visualization:** A fully interactive 3D model of the Rubik's Cube built with **Three.js**, allowing you to scramble the cube and watch the solving algorithm animate the solution in real-time.
* **High-Performance Solving Engine:** The core solving logic is written in modern C++ for maximum performance and efficiency.
* **WebAssembly Integration:** The C++ engine is compiled to **WebAssembly (WASM)**, allowing it to run at near-native speed directly in the browser for a seamless user experience.
* **Extensible Solver Architecture:** The back end uses the **Strategy Design Pattern**, making it easy to add new solving algorithms (like CFOP, Kociemba's, etc.) without altering the core application logic.

---

## Technical Architecture

This project is built on a clean, decoupled architecture that separates the core logic from the presentation layer.

### Back End (C++ Core)

The back end is the heart of the application, responsible for representing the cube's state and executing solving algorithms.

* **Language:** C++17
* **Design:** It follows a clean, object-oriented LLD with a clear separation of concerns:
    * **Model:** `Cube` and `Cubie` classes that accurately represent the physical cube's state.
    * **Controller:** A `CubeController` that manages the application flow and provides a clean public API.
    * **Parser/Translator:** A dedicated component to translate user-friendly move notation (e.g., `"R U' F2"`) into the internal, absolute `Move` commands.
    * **Solver:** An abstract `CubeSolver` class allows for different solving strategies to be implemented and swapped out easily.

### Front End (Web Visualizer)

The front end provides an engaging and intuitive user interface for interacting with the solver.

* **Technologies:** HTML5, CSS3, JavaScript
* **3D Rendering:** The powerful **Three.js** library is used to render the 3D cube, manage camera controls, and create smooth animations for each move.
* **C++ Integration:** The front end communicates with the compiled C++ solver via WebAssembly, sending scramble states to the solver and receiving the solution steps to animate.

---

## How to Build & Run

Detailed instructions for compiling the C++ code with **CMake** and running the web interface are available in the respective `backend/` and `frontend/` directories.