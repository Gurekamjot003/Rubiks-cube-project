@echo off
:: 1. ACTIVATE EMSCRIPTEN
echo --- Activating Emscripten ---
call C:\Users\user\OneDrive\Desktop\coding\emsdk\emsdk_env.bat

:: 2. CREATE WEB BUILD FOLDER
echo.
echo --- Preparing 'build-web' Directory ---
if not exist build-web mkdir build-web
cd build-web

:: 3. CONFIGURE WITH CMAKE AND NINJA FOR WEB
echo.
echo --- Running CMake to Generate Build Files for WebAssembly ---
cmake ../"Back end" -G "Ninja" -DCMAKE_TOOLCHAIN_FILE="C:\Users\user\OneDrive\Desktop\coding\emsdk\upstream\emscripten\cmake\Modules\Platform\Emscripten.cmake"

:: 4. COMPILE WITH NINJA FOR WEB
echo.
echo --- Building the Project with Ninja for WebAssembly ---
cmake --build .

echo.
echo --- Web Build Finished! ---
pause