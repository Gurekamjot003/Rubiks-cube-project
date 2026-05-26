// ==================== EMSCRIPTEN MODULE SETUP ====================
// This Module object is used by Emscripten (WebAssembly compiler) to redirect
// C++ console output to the browser's JavaScript console.
// This allows us to see debug messages from C++ code in the browser console.

var Module = {
    // Capture C++ cout (standard output) and log it to browser console
    print: function(text) {
        if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
        console.log(text);
    },
    // Capture C++ cerr (error output) and log it to browser console
    printErr: function(text) {
        if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
        console.error(text);
    }
};

// Global variable to hold the cube controller object (from C++ WebAssembly)
// This object handles all cube logic like rotations, scrambling, etc.
let cubeEngine = null;

// Global variable to track solution moves and current index
let currentSolution = {
    moves: [],
    currentIndex: 0
};

// Global speed control (1 = normal speed, 2 = 2x speed, 0.5 = half speed)
let moveSpeed = 1;

// Global flag to stop automatic moves
let stopAutomoves = false;

// ==================== PAGE INITIALIZATION ====================
// Wait for the HTML page to fully load before running any code
// This ensures all HTML elements are ready to be accessed
document.addEventListener('DOMContentLoaded', () => {
    const startButton = document.getElementById('startButton');
    if (startButton) {
        // Attach click handler to the start button
        startButton.addEventListener('click', startApp);
    }

    // ==================== THEME TOGGLE LOGIC ====================
    const themeToggleBtn = document.getElementById('themeToggle');
    
    function updateThemeUI(isLight) {
        const emptyOpt = document.getElementById('emptyColorOption');
        if (emptyOpt) {
            emptyOpt.textContent = isLight ? "Dark Gray (Empty)" : "Light Gray (Empty)";
        }
        // If the cube is already initialized, we need to update its visuals
        // so the empty cubies swap between light and dark gray
        if (typeof cubeEngine !== 'undefined' && cubeEngine && typeof create_cube_from_json !== 'undefined') {
            const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
            create_cube_from_json(cube_data);
        }
    }

    if (themeToggleBtn) {
        const iconSpan = themeToggleBtn.querySelector('.icon');
        
        // Check local storage for saved theme
        const savedTheme = localStorage.getItem('cube-theme');
        if (savedTheme === 'light') {
            document.body.classList.add('light-theme');
            if (iconSpan) iconSpan.textContent = '🌙';
            updateThemeUI(true);
        } else {
            updateThemeUI(false);
        }

        themeToggleBtn.addEventListener('click', () => {
            document.body.classList.toggle('light-theme');
            const isLight = document.body.classList.contains('light-theme');
            
            // Update icon and save preference
            if (isLight) {
                if (iconSpan) iconSpan.textContent = '🌙';
                localStorage.setItem('cube-theme', 'light');
            } else {
                if (iconSpan) iconSpan.textContent = '☀️';
                localStorage.setItem('cube-theme', 'dark');
            }
            
            updateThemeUI(isLight);
        });
    }
});

// This callback is called after WebAssembly (C++ code) finishes loading and initializing
// It's used to enable the start button once everything is ready
Module.onRuntimeInitialized = function() {
    const startButton = document.getElementById('startButton');
    if (startButton) {
        // Enable the button (remove disabled attribute)
        startButton.disabled = false;
        startButton.textContent = "Start";
    }
};

// ==================== MAIN APPLICATION STARTUP ====================
// This function runs when the user clicks the "Start" button.
// It initializes the entire application: 3D graphics, cube, and event handlers
function startApp() {
    // Hide the entry/welcome page
    document.getElementById('entry-page').style.display = 'none';
    // Show the main game page
    document.getElementById('main-page').style.display = 'flex';

    // Get the HTML container where the 3D canvas will be placed
    const cubeContainer = document.getElementById('canvas-container');
    // Initialize the 3D graphics engine (Three.js)
    init_3d_cube(cubeContainer);

    // Create a new cube from C++ (3x3x3 by default)
    initCube();

    // ==================== ATTACH EVENT LISTENERS ====================
    // These listeners respond to user actions (button clicks, keyboard input)

    // Scramble button: randomly shuffle the cube
    document.getElementById('scrambleButton').addEventListener('click', scramble);

    // Reset button: create a new solved cube
    document.getElementById('resetButton').addEventListener('click', () => {
        const n = parseInt(document.getElementById('cubeSizeInput').value);
        // Animate camera back to default view
        animateToDefault(n, 300, () => {
            // After animation, create a new cube
            initCube();
        });
    });

    // Apply Move button: execute the move entered in the text field
    document.getElementById('applyMoveButton').addEventListener('click', applyMove);

    // Reset Camera button: view the cube from the default angle
    document.getElementById('resetCameraButton').addEventListener('click', resetCamera);

    // Solve button: attempt to solve the cube (not implemented yet)
    document.getElementById('solveButton').addEventListener('click', solve);

    // Apply color button
    document.getElementById('applyColorButton').addEventListener('click', applyCubieColor);

    // Edit Cube Panel Toggle Buttons
    document.getElementById('editCubeButton').addEventListener('click', () => {
        document.getElementById('color-picker-panel').classList.remove('hidden');
        document.getElementById('color-picker-panel').style.display = 'block';
    });
    
    document.getElementById('closeColorPickerButton').addEventListener('click', () => {
        document.getElementById('color-picker-panel').classList.add('hidden');
        document.getElementById('color-picker-panel').style.display = 'none';
    });

    document.getElementById('clearCubeButton').addEventListener('click', clearCubeColors);

    // Solution panel buttons
    document.getElementById('applyNextMoveButton').addEventListener('click', applyNextMove);
    document.getElementById('applyAllMovesButton').addEventListener('click', applyAllMoves);
    document.getElementById('closeSolutionButton').addEventListener('click', closeSolution);
    
    // Speed control buttons
    document.getElementById('speedIncreaseButton').addEventListener('click', increaseSpeed);
    document.getElementById('speedDecreaseButton').addEventListener('click', decreaseSpeed);
    
    // Custom moves and stop buttons
    document.getElementById('applyCustomMovesButton').addEventListener('click', applyCustomMoves);
    document.getElementById('stopButton').addEventListener('click', stopMoves);
    
    // Allow pressing Enter to apply custom moves
    document.getElementById('customMovesInput').addEventListener('keydown', (e) => {
        if (e.key === 'Enter') {
            applyCustomMoves();
        }
    });
    
    // Keyboard shortcuts for speed control
    document.addEventListener('keydown', (e) => {
        if (e.key === '+' || e.key === '=') {
            e.preventDefault();
            increaseSpeed();
        } else if (e.key === '-' || e.key === '_') {
            e.preventDefault();
            decreaseSpeed();
        }
    });

    // Keyboard: allow pressing Enter to apply a move (instead of clicking the button)
    document.getElementById('moveInput').addEventListener('keydown', (e) => {
        if (e.key === 'Enter') {
            applyMove();
        }
    });
}

// ==================== VIEW ANIMATION ====================
// Smoothly animates the camera and cube back to their default positions
// This creates a nice transition effect when resetting or loading a new cube
//
// Parameters:
//   n - Cube size (used to calculate default camera distance)
//   duration - How long the animation should take in milliseconds (default: 500ms)
//   cb - Optional callback function to run when animation completes
function animateToDefault(n, duration = 500, cb) {
    // If no 3D cube exists yet, just call the callback and exit
    if (!cubeParent) {
        if (cb) cb();
        return;
    }

    // Save the current camera position (we'll animate from this)
    const startCamPos = camera.position.clone();
    // Calculate the default camera position based on cube size
    // The camera is positioned diagonally from the cube for a nice isometric view
    const defaultCamPos = new THREE.Vector3(n * 2.0, n * 2.0 * 0.75, n * 2.0);

    // Save the current cube rotation (we'll animate from this)
    const startQuat = cubeParent.quaternion.clone();
    // Default rotation is identity (no rotation)
    const defaultQuat = new THREE.Quaternion(0, 0, 0, 1);

    // Record when this animation starts (used to calculate progress)
    const startTime = performance.now();

    // This function is called every frame while the animation is running
    function step(time) {
        // Calculate animation progress: 0 = start, 1 = complete
        const t = Math.min(1, (time - startTime) / duration);

        // Interpolate (smoothly blend) the camera position
        // lerpVectors = linear interpolation between two vectors
        camera.position.lerpVectors(startCamPos, defaultCamPos, t);
        // Point the camera at the scene center
        camera.lookAt(scene.position);

        // Interpolate the cube rotation (slerp = spherical linear interpolation)
        // This smoothly blends between rotations without gimbal lock
        cubeParent.quaternion.slerpQuaternions(startQuat, defaultQuat, t);

        // If animation is not complete, schedule the next frame
        if (t < 1) {
            requestAnimationFrame(step);
        } else {
            // Animation complete! Reset inertia (cube stops spinning)
            inertiaRotation.set(0, 0, 0, 1);
            // Call the callback function if provided
            if (cb) cb();
        }
    }
    // Start the animation loop
    requestAnimationFrame(step);
}


// ==================== CAMERA CONTROL ====================
// Resets the camera view to the default angle without animation
// This is called when the user clicks the "Reset Camera" button
function resetCamera() {
    const n = parseInt(document.getElementById('cubeSizeInput').value);
    // Animate back to default view with 300ms transition
    animateToDefault(n, 300); // No callback: just move view back
}

// ==================== CUBE INITIALIZATION ====================
// Creates a new cube from the C++ backend and renders it in 3D
// This is called when starting the app or when the user clicks "Reset"
function initCube() {
    // If a cube already exists, delete it to free memory
    if (cubeEngine) {
        cubeEngine.delete();
    }

    // Get the cube size from the input field (e.g., "3" for 3x3x3 cube)
    const n = parseInt(document.getElementById('cubeSizeInput').value);

    // Validate that the cube size is valid
    if (isNaN(n) || n < 2) {
        alert("Please enter a valid cube size (2 or greater).");
        return;
    }

    try {
        // Create a new cube controller object from the C++ WebAssembly code
        // This object handles all the cube logic (moves, state, etc.)
        cubeEngine = new Module.CubeController(n);
        console.log(`Created ${n}x${n}x${n} Cube!`);

        // Show/hide solve controls based on cube size
        const solveGroup = document.getElementById('solve-control-group');
        if (solveGroup) {
            if (n === 3) {
                solveGroup.classList.remove('hidden');
                solveGroup.style.display = 'flex';
            } else {
                solveGroup.classList.add('hidden');
                solveGroup.style.display = 'none';
                // Also close solution panel if it was open
                closeSolution();
            }
        }

        // Animate the camera back to the default view first
        // This ensures a smooth transition when changing cube sizes
        animateToDefault(n, 300, () => {
            // After animation completes, fetch the cube state from C++ as JSON
            const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
            // Create the 3D visual representation of the cube
            create_cube_from_json(cube_data);
        });

    } catch(e) {
        // If something goes wrong, log the error and show an alert
        console.error(e);
        alert("Error initializing cube. Check console.");
    }
}

// ==================== MOVE EXECUTION ====================
// Duration for animating a single move (in milliseconds)
// 500ms = half second per move
const MOVE_ANIMATION_DURATION = 500;

// Converts view rotation moves (x, y, z) to equivalent layer moves
// x, y, z rotate the entire cube by expanding to all layer rotations
function expandViewRotationMove(moveToken, cubeSize) {
    const lowerToken = moveToken.toLowerCase();

    // Check if it's a view rotation move (x, y, or z)
    if (!lowerToken.startsWith('x') && !lowerToken.startsWith('y') && !lowerToken.startsWith('z')) {
        return [moveToken]; // Not a view rotation, return as-is
    }

    // Extract the axis and suffix (', 2, etc.)
    const axis = moveToken[0].toLowerCase();
    const suffix = moveToken.slice(1); // Everything after the axis letter

    // Map axis to face
    const equivalentFaceMap = { 'x': 'R', 'y': 'U', 'z': 'F' };
    const face = equivalentFaceMap[axis];

    // Generate moves for each layer
    // For a 3x3 cube: "x" becomes ["R", "2R", "3R"]
    const moves = [];
    for (let layer = 1; layer <= cubeSize; layer++) {
        const moveStr = layer === 1 ? `${face}${suffix}` : `${layer}${face}${suffix}`;
        moves.push(moveStr);
    }

    return moves;
}

// Checks if a move is a view rotation (x, y, z)
function isViewRotationMove(moveToken) {
    const lower = moveToken.toLowerCase();
    return lower.startsWith('x') || lower.startsWith('y') || lower.startsWith('z');
}

// Generates a single wide-type animation for view rotations
function generateViewRotationAnimation(moveToken, cubeSize) {
    const axis = moveToken[0].toLowerCase();
    const suffix = moveToken.slice(1);

    // Determine rotation count based on suffix
    let rotations = 1;
    if (suffix === "'") rotations = 3; // ' = 3 rotations (270°)
    if (suffix === "2") rotations = 2;

    // Map axis to animation axis
    const axisMap = { 'x': 'X', 'y': 'Y', 'z': 'Z' };
    const animAxis = axisMap[axis];

    // Return a single wide animation for all layers rotating simultaneously
    return [{
        type: "wide",
        axis: animAxis,
        layer: 0,
        rotations: rotations,
        num_layers: cubeSize,
        simultaneous: true
    }];
}

// ==================== UNIFIED MOVE APPLICATION WITH ANIMATION ====================
// Helper function to apply a single move with animation
// This is used by both user input (applyMove) and solver moves (applyNextMove, applyAllMoves)
//
// Parameters:
//   moveToken - The move to apply (e.g., "R", "U'", "x", "2R", etc.)
//   callback - Function to call after the move is applied and animated
function applySingleMoveWithAnimation(moveToken, callback) {
    if (!cubeEngine) {
        if (callback) callback();
        return;
    }

    const cubeSize = parseInt(document.getElementById('cubeSizeInput').value);
    // Calculate animation duration based on move speed
    // Higher speed = shorter duration (inversely proportional)
    const adjustedDuration = MOVE_ANIMATION_DURATION / moveSpeed;

    try {
        if (isViewRotationMove(moveToken)) {
            // Handle view rotation moves (x, y, z) with simultaneous animation
            const animations = generateViewRotationAnimation(moveToken, cubeSize);
            const expandedMoves = expandViewRotationMove(moveToken, cubeSize);

            // Animate with simultaneous flag
            animateSequence(animations, adjustedDuration, () => {
                // Apply all layer moves that make up this view rotation
                for (const layerMove of expandedMoves) {
                    cubeEngine.apply_move(layerMove);
                }

                // Get the new cube state and re-render
                const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
                create_cube_from_json(cube_data);

                // Call the callback when done
                if (callback) callback();
            });
        } else {
            // Regular move - get animation info and animate
            if (typeof cubeEngine.get_move_animation_info === 'function') {
                const infoJSON = cubeEngine.get_move_animation_info(moveToken);
                const animations = JSON.parse(infoJSON);

                // Animate the move on screen
                animateSequence(animations.slice(), adjustedDuration, () => {
                    // After animation is done, update the internal cube state in C++
                    cubeEngine.apply_move(moveToken);

                    // Get the new cube state from C++ and re-render the cube
                    const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
                    create_cube_from_json(cube_data);

                    // Call the callback when done
                    if (callback) callback();
                });
            } else {
                // Fallback: if animation info is not available, just apply the move without animation
                cubeEngine.apply_move(moveToken);
                const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
                create_cube_from_json(cube_data);
                if (callback) callback();
            }
        }
    } catch (e) {
        console.error(`Error processing move "${moveToken}":`, e);
        if (callback) callback();
    }
}

// Applies a sequence of moves from user input with animation
// The move is read from the input field, animated on screen, and applied to the cube state
function applyMove() {
    // Get the move string from the input field and remove extra whitespace
    const move = document.getElementById('moveInput').value.trim();

    // Exit if no cube exists or no move was entered
    if (!cubeEngine || !move) return;

    console.log(`> Apply Move: ${move}`);

    // Get cube size for view rotation conversion
    const cubeSize = parseInt(document.getElementById('cubeSizeInput').value);

    // Split the move string into individual moves
    const rawTokens = move.split(/\s+/);

    let idx = 0;

    // This function processes moves one at a time
    function doNext() {
        // If all moves have been processed, clear the input and finish
        if (idx >= rawTokens.length) {
            document.getElementById('moveInput').value = '';
            return;
        }

        const moveToken = rawTokens[idx++];

        // Apply the move with animation
        applySingleMoveWithAnimation(moveToken, doNext);
    }

    try {
        // Start processing moves
        doNext();
    } catch (e) {
        console.error(e);
    }
}

// ==================== CUBE MANIPULATION ====================
// Randomly scrambles (shuffles) the cube
// Called when the user clicks the "Scramble" button
function scramble() {
    if(!cubeEngine) return;
    console.log(`> Scrambling...`);

    // Call the C++ scramble function
    cubeEngine.scramble_cube(20);

    // Get the cube size
    const n = parseInt(document.getElementById('cubeSizeInput').value);

    // Animate the camera back to default view, then re-render the scrambled cube
    animateToDefault(n, 300, () => {
        // Fetch the scrambled cube state from C++
        const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
        // Render the scrambled cube in 3D
        create_cube_from_json(cube_data);
    });
}

// ==================== LOADING INDICATOR HELPERS ====================
// Show the loading overlay with spinner and text
function showLoadingOverlay() {
    const overlay = document.getElementById('solve-loading-overlay');
    if (overlay) {
        overlay.classList.remove('hidden');
    }
}

// Hide the loading overlay
function hideLoadingOverlay() {
    const overlay = document.getElementById('solve-loading-overlay');
    if (overlay) {
        overlay.classList.add('hidden');
    }
}

// Attempts to solve the cube using the selected algorithm
// Called when the user clicks the "Solve" button
function solve() {
    if(!cubeEngine) return;

    // Get the selected solver algorithm from the dropdown
    const algo = document.getElementById('solverSelect').value;
    console.log(`> Solving with ${algo}...`);
    
    // Show loading overlay
    showLoadingOverlay();

    try {
        // Call C++ method to get solution moves as JSON
        const solutionJSON = cubeEngine.get_solution_JSON(algo);
        const solution = JSON.parse(solutionJSON);
        
        // Hide loading overlay once solution is found
        hideLoadingOverlay();
        
        // Split moves on spaces to ensure each move is individual
        const allMoves = [];
        solution.moves.forEach(moveStr => {
            const splitMoves = moveStr.trim().split(/\s+/);
            allMoves.push(...splitMoves);
        });
        
        // Store the solution moves globally (with split moves)
        currentSolution.moves = allMoves;
        currentSolution.currentIndex = 0;
        
        // Update the solution object with split moves for display
        solution.moves = allMoves;
        solution.moveCount = allMoves.length;
        
        // Log solution info with timing if available
        if (solution.solveTimeMs !== undefined) {
            console.log(`Solution found with ${solution.moveCount} moves in ${solution.solveTimeMs.toFixed(2)}ms`);
        } else {
            console.log(`Solution found with ${solution.moveCount} moves`);
        }
        displaySolution(solution);
        
        // Show the solution panel
        document.getElementById('solution-panel').style.display = 'block';
    } catch (error) {
        // Hide loading overlay on error
        hideLoadingOverlay();
        console.error('Error solving cube:', error);
        alert('Error solving cube: ' + error.message);
    }
}

// Display the solution moves in the UI
function displaySolution(solution) {
    const solutionInfo = document.getElementById('solution-info');
    const solutionMoves = document.getElementById('solution-moves');
    
    // Format the solve time: show in ms if less than 1000ms, otherwise convert to seconds
    let timeDisplay = '';
    if (solution.solveTimeMs !== undefined) {
        if (solution.solveTimeMs < 1000) {
            timeDisplay = `<span class="solve-time">⏱ Solved in ${solution.solveTimeMs.toFixed(2)}ms</span>`;
        } else {
            const seconds = (solution.solveTimeMs / 1000).toFixed(2);
            timeDisplay = `<span class="solve-time">⏱ Solved in ${seconds}s</span>`;
        }
    }
    
    // Display move count, progress, and solve time
    solutionInfo.innerHTML = `<p>Total moves: <strong>${solution.moveCount}</strong> | Current: <strong>0/${solution.moveCount}</strong></p>${timeDisplay}`;
    
    // Display moves as a list with styling for current move
    let movesHTML = '<div class="moves-list"><p>';
    solution.moves.forEach((move, index) => {
        if (index === 0) {
            movesHTML += `<span class="move current-move">${move}</span>`;
        } else {
            movesHTML += `<span class="move">${move}</span>`;
        }
        if ((index + 1) % 10 === 0) {
            movesHTML += '</p><p>';
        } else {
            movesHTML += ' ';
        }
    });
    movesHTML += '</p></div>';
    solutionMoves.innerHTML = movesHTML;
}

// Apply the next move in the solution with animation
function applyNextMove() {
    if (currentSolution.moves.length === 0) return;
    
    if (currentSolution.currentIndex >= currentSolution.moves.length) {
        alert('All moves have been applied!');
        return;
    }
    
    const move = currentSolution.moves[currentSolution.currentIndex];
    console.log(`Applying move ${currentSolution.currentIndex + 1}: ${move}`);
    
    // Increment the index before animation (so visual highlighting is immediate)
    currentSolution.currentIndex++;
    
    // Update display immediately (before animation)
    const total = currentSolution.moves.length;
    const info = document.getElementById('solution-info');
    info.innerHTML = `<p>Total moves: <strong>${total}</strong> | Current: <strong>${currentSolution.currentIndex}/${total}</strong></p>`;
    highlightCurrentMove();
    
    // Apply the move with animation using the unified function
    applySingleMoveWithAnimation(move, () => {
        // Animation complete - display already updated above
    });
}

// Apply all remaining moves automatically with animation
function applyAllMoves() {
    if (currentSolution.moves.length === 0) return;
    
    console.log(`Applying all ${currentSolution.moves.length} moves...`);
    
    stopAutomoves = false;
    document.getElementById('stopButton').classList.remove('hidden');
    
    let moveIndex = currentSolution.currentIndex;
    
    // Function to apply moves sequentially with animation
    function applyNextMoveInSequence() {
        if (stopAutomoves || moveIndex >= currentSolution.moves.length) {
            // All moves applied or stopped
            currentSolution.currentIndex = moveIndex;
            const total = currentSolution.moves.length;
            const info = document.getElementById('solution-info');
            info.innerHTML = `<p>Total moves: <strong>${total}</strong> | Current: <strong>${currentSolution.currentIndex}/${total}</strong></p>`;
            highlightCurrentMove();
            document.getElementById('stopButton').classList.add('hidden');
            stopAutomoves = false;
            if (!stopAutomoves) {
                console.log('All moves applied!');
            } else {
                console.log('Moves stopped');
            }
            return;
        }
        
        const move = currentSolution.moves[moveIndex];
        moveIndex++;
        currentSolution.currentIndex = moveIndex; // Update immediately for real-time display
        
        // Update display immediately (before animation)
        const total = currentSolution.moves.length;
        const info = document.getElementById('solution-info');
        info.innerHTML = `<p>Total moves: <strong>${total}</strong> | Current: <strong>${currentSolution.currentIndex}/${total}</strong></p>`;
        highlightCurrentMove();
        
        // Apply the move with animation
        applySingleMoveWithAnimation(move, () => {
            // After animation, continue with next move
            applyNextMoveInSequence();
        });
    }
    
    // Start applying moves
    applyNextMoveInSequence();
}

// Update the visual highlighting of the current move in the list
function highlightCurrentMove() {
    const moveElements = document.querySelectorAll('.moves-list .move');
    moveElements.forEach((el, index) => {
        if (index === currentSolution.currentIndex) {
            el.classList.add('current-move');
        } else {
            el.classList.remove('current-move');
        }
        
        if (index < currentSolution.currentIndex) {
            el.classList.add('applied-move');
        } else {
            el.classList.remove('applied-move');
        }
    });
}

// Close the solution panel
function closeSolution() {
    document.getElementById('solution-panel').style.display = 'none';
    currentSolution.moves = [];
    currentSolution.currentIndex = 0;
    stopAutomoves = false;
    document.getElementById('stopButton').classList.add('hidden');
}

// ==================== SPEED CONTROL ====================
// Speed options: 0.25x, 0.5x, 0.75x, 1x, 2x, 4x, 8x, 16x
const SPEED_OPTIONS = [0.25, 0.5, 0.75, 1, 2, 4, 8, 16];

// Increase the move speed
function increaseSpeed() {
    const currentIndex = SPEED_OPTIONS.indexOf(moveSpeed);
    if (currentIndex < SPEED_OPTIONS.length - 1) {
        moveSpeed = SPEED_OPTIONS[currentIndex + 1];
        updateSpeedDisplay();
    }
}

// Decrease the move speed
function decreaseSpeed() {
    const currentIndex = SPEED_OPTIONS.indexOf(moveSpeed);
    if (currentIndex > 0) {
        moveSpeed = SPEED_OPTIONS[currentIndex - 1];
        updateSpeedDisplay();
    }
}

// Update the speed display text
function updateSpeedDisplay() {
    const display = document.getElementById('speedDisplay');
    // Format speed to remove unnecessary decimal places
    const formatted = moveSpeed < 1 ? moveSpeed.toFixed(2) : moveSpeed.toString();
    display.textContent = formatted + 'x';
}

// Apply custom number of moves
function applyCustomMoves() {
    const input = document.getElementById('customMovesInput');
    const numMoves = parseInt(input.value);
    
    if (isNaN(numMoves) || numMoves < 1) {
        alert('Please enter a valid number of moves');
        return;
    }
    
    if (currentSolution.moves.length === 0) {
        alert('No solution loaded');
        return;
    }
    
    if (currentSolution.currentIndex >= currentSolution.moves.length) {
        alert('All moves have been applied!');
        return;
    }
    
    console.log(`Applying ${numMoves} moves...`);
    stopAutomoves = false;
    document.getElementById('stopButton').classList.remove('hidden');
    
    let moveIndex = currentSolution.currentIndex;
    let movesApplied = 0;
    
    // Function to apply moves sequentially with speed control
    function applyNextMoveInCustomSequence() {
        if (stopAutomoves || moveIndex >= currentSolution.moves.length || movesApplied >= numMoves) {
            // Finished applying custom moves
            currentSolution.currentIndex = moveIndex;
            const total = currentSolution.moves.length;
            const info = document.getElementById('solution-info');
            info.innerHTML = `<p>Total moves: <strong>${total}</strong> | Current: <strong>${currentSolution.currentIndex}/${total}</strong></p>`;
            highlightCurrentMove();
            document.getElementById('stopButton').classList.add('hidden');
            stopAutomoves = false;
            input.value = '';
            console.log(`Applied ${movesApplied} moves!`);
            return;
        }
        
        const move = currentSolution.moves[moveIndex];
        moveIndex++;
        movesApplied++;
        currentSolution.currentIndex = moveIndex; // Update immediately for real-time display
        
        // Update display immediately (before animation)
        const total = currentSolution.moves.length;
        const info = document.getElementById('solution-info');
        info.innerHTML = `<p>Total moves: <strong>${total}</strong> | Current: <strong>${currentSolution.currentIndex}/${total}</strong></p>`;
        highlightCurrentMove();
        
        // Apply the move with animation adjusted for speed
        applySingleMoveWithAnimation(move, () => {
            // After animation, continue with next move
            applyNextMoveInCustomSequence();
        });
    }
    
    // Start applying moves
    applyNextMoveInCustomSequence();
}

// Stop applying moves
function stopMoves() {
    stopAutomoves = true;
    document.getElementById('stopButton').classList.add('hidden');
    console.log('Moves stopped');
}

// ==================== CUBIE COLORING ====================
// Applies a specific color to a cubie face at the given coordinates
function applyCubieColor() {
    if (!cubeEngine) return;

    const faceStr = document.getElementById('colorFaceSelect').value;
    const colorStr = document.getElementById('colorSelect').value;
    const x = parseInt(document.getElementById('colorXInput').value);
    const y = parseInt(document.getElementById('colorYInput').value);

    // Validate inputs
    const cubeSize = parseInt(document.getElementById('cubeSizeInput').value);
    if (isNaN(x) || isNaN(y) || x < 0 || y < 0 || x >= cubeSize || y >= cubeSize) {
        alert(`Please enter valid X and Y coordinates between 0 and ${cubeSize - 1}.`);
        return;
    }

    try {
        const faceEnum = Module.FaceEnum[faceStr];
        const colorEnum = Module.Color[colorStr];

        console.log(`Setting color ${colorStr} at Face: ${faceStr}, X: ${x}, Y: ${y}`);
        cubeEngine.set_cubie_color(faceEnum, x, y, colorEnum);

        // Fetch the updated cube state from C++ and re-render
        const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
        create_cube_from_json(cube_data);
    } catch (e) {
        console.error("Error setting cubie color:", e);
        alert("Failed to set cubie color. Check console for details.");
    }
}

// Clears all colors from the cube by setting them to EMPTY
function clearCubeColors() {
    if (!cubeEngine) return;

    const cubeSize = parseInt(document.getElementById('cubeSizeInput').value);
    const faces = ['UP', 'DOWN', 'LEFT', 'RIGHT', 'FRONT', 'BACK'];
    const colorEnum = Module.Color['EMPTY'];

    try {
        console.log("Clearing all cube colors...");
        // Iterate over all faces and all coordinates
        for (const faceStr of faces) {
            const faceEnum = Module.FaceEnum[faceStr];
            for (let i = 0; i < cubeSize; i++) {
                for (let j = 0; j < cubeSize; j++) {
                    cubeEngine.set_cubie_color(faceEnum, i, j, colorEnum);
                }
            }
        }
        
        // Fetch the updated cube state from C++ and re-render
        const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
        create_cube_from_json(cube_data);
    } catch (e) {
        console.error("Error clearing cube colors:", e);
        alert("Failed to clear cube colors. Check console for details.");
    }
}
