var Module = {
    // Redirect C++ cout to this HTML div
    print: (function() {
        var element = document.getElementById('output');
        if (element) element.innerHTML = ''; // clear initial text
        return function(text) {
            if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
            console.log(text);
            if (element) {
                // rudimentary ANSI color parsing could go here, for now just text
                element.innerHTML += text + "\n";
                element.scrollTop = element.scrollHeight; // auto scroll
            }
        };
    })()
};



let cubeEngine = null;

// Wait for Emscripten to finish loading
Module.onRuntimeInitialized = function() {
    document.getElementById('output').innerHTML = "✅ WASM Loaded! Ready to initialize cube.";
    document.getElementById('initButton').disabled = false;
    init_3d_cube(document.getElementById('cube-container'));
};

function initCube() {
    if (cubeEngine) {
        cubeEngine.delete(); // Prevent memory leaks! C++ isn't garbage collected.
    }
    
    const n = parseInt(document.getElementById('cubeSize').value);
    try {
        // Create C++ Object instance
        cubeEngine = new Module.CubeController(n);
        
        // Enable controls
        const controls = document.getElementById('gameControls');
        controls.style.display = "block";
        controls.style.opacity = "1";
        controls.style.pointerEvents = "auto";
        
        document.getElementById('output').innerHTML = `Created ${n}x${n}x${n} Cube!\n`;
        
        const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
        console.log(cube_data);
        create_cube_from_json(cube_data);
        cubeEngine.display_cube();
    } catch(e) {
        console.error(e);
        alert("Error initializing cube. Check console.");
    }
}


function applyMove() {
    const move = document.getElementById('moveInput').value;
    if(!cubeEngine || !move) return;
    
    document.getElementById('output').innerHTML += `\n> Apply Move: ${move}\n`;
    
    try {
        cubeEngine.apply_move(move);
        const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
        create_cube_from_json(cube_data);
        cubeEngine.display_cube();
        document.getElementById('moveInput').value = ''; // clear input
    } catch(e) {
        document.getElementById('output').innerHTML += `❌ Error: ${e}\n`;
    }
}

function scramble() {
    if(!cubeEngine) return;
    document.getElementById('output').innerHTML += `\n> Scrambling...\n`;
    cubeEngine.scramble_cube();
    const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
    create_cube_from_json(cube_data);
    cubeEngine.display_cube();
}

function display() {
    if(cubeEngine) {
        const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
        create_cube_from_json(cube_data);
        cubeEngine.display_cube();
    }
}

window.initCube = initCube;
window.applyMove = applyMove;
window.scramble = scramble;
window.display = display;
