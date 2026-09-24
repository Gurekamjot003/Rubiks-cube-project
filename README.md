# C++ Rubik's Cube Solver & 3D Visualizer

A high-performance 3x3x3 Rubik's Cube engine and Layer-by-Layer (LBL) solver, written in modern C++17, paired with an interactive 3D web visualizer built with Three.js and WebAssembly.

---

## Overview

This project implements a full Rubik's Cube state engine and a deterministic layer-by-layer solving algorithm from scratch in C++, then exposes it to the browser via WebAssembly so the solve can be watched move-by-move in a real-time 3D visualization.

- **Solve performance:** resolves arbitrary scrambles in ~200–300 moves at an average execution time of ~100 ms.
- **Codebase size:** 2,100+ lines of C++ across the state engine, solver, and command/translation layers.
- **Architecture:** built as a B.Tech capstone project, with an emphasis on clean object-oriented design, memory safety, and deterministic execution rather than just "getting a solve to work."

---

## Core Features

- **Interactive 3D Visualization** — a fully interactive 3D cube built with Three.js; scramble the cube and watch the solver's moves animate in real time.
- **High-Performance Solving Engine** — the core LBL solver is written in modern C++ for speed and efficiency, resolving scrambles in ~200–300 moves at ~100 ms average.
- **WebAssembly Integration** — the C++ engine compiles to WebAssembly (WASM), running at near-native speed directly in the browser.
- **Extensible Solver Architecture** — solving strategies are abstracted so new algorithms (e.g. CFOP, Kociemba's) can be added without touching core application logic.
- **Orientation-Independent Move Translation** — a dedicated translation layer maps equivalent user moves across different camera orientations into a common internal representation, keeping camera orientation fully decoupled from cube state and enabling O(1) view rotations.

---

## Technical Architecture

The project separates core logic from the presentation layer across two independently buildable components.

### Back End (C++ Core) — `Back end/`

- **Language:** C++17
- **Design principles:** SOLID, DRY, KISS
- **Design patterns used:**
  - **Strategy** — abstracts the solving algorithm so different solvers can be swapped in without changing the controller or model.
  - **Factory** — handles construction of cube/cubie objects and move instances.
  - **Command** — encapsulates each move as a command object for parsing, execution, and translation.
  - **Adapter** — bridges the internal move representation with the WASM-facing API consumed by the front end.
  - **Facade** — the `CubeController` exposes a simplified public API over the underlying model/solver subsystems.
  - **Singleton** — used where a single shared instance of a core manager is required.
- **Key components:**
  - **Model** — `Cube` and `Cubie` classes representing the physical cube's state as a 3D array of cubie objects (rather than a flat sticker-based representation), enabling realistic spatial transformations and a scalable architecture that generalizes to N×N cubes.
  - **Controller** — `CubeController`, providing a clean public API and managing application flow.
  - **Parser / Translator** — converts user-friendly move notation (e.g. `"R U' F2"`) into internal, absolute `Move` commands, independent of camera orientation.
  - **Solver** — an abstract `CubeSolver` base class with a concrete Layer-by-Layer implementation; face rotations are implemented in-place using the classic matrix rotation algorithm (O(n³) to identify affected cubies, O(n²) to rotate a layer, constant auxiliary space), using STL containers and `std::unique_ptr` for memory-safe resource ownership.

### Front End (Web Visualizer) — `Front end/`

- **Technologies:** HTML5, CSS3, JavaScript
- **3D rendering:** Three.js renders the cube, manages camera controls, and animates each move smoothly.
- **C++ integration:** communicates with the compiled WebAssembly module — sending scramble states to the solver and receiving solution steps to animate.

---

## How to Build & Run

> Full details live in the `Back end/` and `Front end/` directories; summary below.

1. **Back end (C++ → WebAssembly):**
   - Requires CMake and an Emscripten toolchain for the WASM build.
   - From the repo root, run `build.bat` (Windows) to compile the C++ core.
2. **Front end:**
   - Open the `Front end/` directory's entry HTML file in a browser, or serve it with a local static server.
   - The page loads the compiled WASM module and connects it to the Three.js visualizer.

*(If your actual build steps differ — e.g. specific CMake flags, Emscripten version, or a dev server command — swap this section for the exact commands so anyone cloning the repo can build it without guessing.)*

---

## Project Status

Completed as a B.Tech capstone project (Jan 2026 – May 2026) under faculty guidance. Currently a single solving strategy (Layer-by-Layer) is implemented; the abstracted solver architecture is designed to support additional algorithms in the future.

---

## Tech Stack

`C++17` · `STL` · `CMake` · `WebAssembly (Emscripten)` · `Three.js` · `HTML5` · `CSS3` · `JavaScript`
