@echo off
setlocal enabledelayedexpansion

:: Suppress Emscripten verbose output
set EMSDK_QUIET=1

:: 1. ACTIVATE EMSCRIPTEN
echo --- Activating Emscripten ---
call C:\Users\user\OneDrive\Desktop\coding\emsdk\emsdk_env.bat

:: 2. CREATE WEB BUILD FOLDER
echo.
echo --- Preparing 'build-web' Directory ---
if not exist build-web mkdir build-web
cd build-web

:: 3. CONFIGURE WITH CMAKE AND NINJA FOR WEB (only if not already configured)
echo.
if not exist CMakeCache.txt (
    echo --- Running CMake to Generate Build Files for WebAssembly ---
    cmake ../"Back end" -G "Ninja" -DCMAKE_TOOLCHAIN_FILE="C:\Users\user\OneDrive\Desktop\coding\emsdk\upstream\emscripten\cmake\Modules\Platform\Emscripten.cmake" -Wno-dev
    if errorlevel 1 (
        echo ERROR: CMake configuration failed!
        pause
        exit /b 1
    )
) else (
    echo --- CMake already configured, skipping reconfiguration ---
)

:: 4. COMPILE WITH NINJA FOR WEB (using all CPU cores)
echo.
echo --- Building the Project with Ninja for WebAssembly ---
cmake --build . --parallel

if errorlevel 1 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo --- Web Build Finished Successfully! ---
echo Output: RubiksCube.js and RubiksCube.wasm
pause