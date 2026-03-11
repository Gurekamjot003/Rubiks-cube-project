var Module = {
    // Redirect C++ cout to the browser console
    print: function(text) {
        if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
        console.log(text);
    },
    printErr: function(text) {
        if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
        console.error(text);
    }
};

let cubeEngine = null;

// NEW: Main application startup logic
document.addEventListener('DOMContentLoaded', () => {
    const startButton = document.getElementById('startButton');
    if (startButton) {
        startButton.addEventListener('click', startApp);
    }
});

Module.onRuntimeInitialized = function() {
    const startButton = document.getElementById('startButton');
    if (startButton) {
        startButton.disabled = false;
        startButton.textContent = "Start";
    }
};

function startApp() {
    // Hide entry page, show main page
    document.getElementById('entry-page').style.display = 'none';
    document.getElementById('main-page').style.display = 'flex';

    const cubeContainer = document.getElementById('canvas-container');
    init_3d_cube(cubeContainer);

    initCube();

    // Setup new event listeners
    document.getElementById('scrambleButton').addEventListener('click', scramble);
    document.getElementById('resetButton').addEventListener('click', () => {
        const n = parseInt(document.getElementById('cubeSizeInput').value);
        animateToDefault(n, 300, () => {
            initCube();
        });
    }); // Reset just re-initializes
    document.getElementById('applyMoveButton').addEventListener('click', applyMove);
    document.getElementById('resetCameraButton').addEventListener('click', resetCamera);
    document.getElementById('solveButton').addEventListener('click', solve);
    document.getElementById('moveInput').addEventListener('keydown', (e) => {
        if (e.key === 'Enter') {
            applyMove();
        }
    });
}

// animate camera and cubeParent back to the default view for size n, then call cb
function animateToDefault(n, duration = 500, cb) {
    // if no callback provided, just animate the view; cb optional
    if (!cubeParent) {
        if (cb) cb();
        return;
    }

    const startCamPos = camera.position.clone();
    const defaultCamPos = new THREE.Vector3(n * 2.0, n * 2.0 * 0.75, n * 2.0);
    const startQuat = cubeParent.quaternion.clone();
    const defaultQuat = new THREE.Quaternion(0, 0, 0, 1);

    const startTime = performance.now();
    function step(time) {
        const t = Math.min(1, (time - startTime) / duration);
        camera.position.lerpVectors(startCamPos, defaultCamPos, t);
        camera.lookAt(scene.position);
        cubeParent.quaternion.slerpQuaternions(startQuat, defaultQuat, t);
        if (t < 1) {
            requestAnimationFrame(step);
        } else {
            inertiaRotation.set(0, 0, 0, 1);
            if (cb) cb();
        }
    }
    requestAnimationFrame(step);
}


function resetCamera() {
    const n = parseInt(document.getElementById('cubeSizeInput').value);
    animateToDefault(n, 300); // no cb: just move view back
}

function initCube() {
    if (cubeEngine) {
        cubeEngine.delete();
    }
    
    const n = parseInt(document.getElementById('cubeSizeInput').value);
    if (isNaN(n) || n < 2) {
        alert("Please enter a valid cube size (2 or greater).");
        return;
    }

    try {
        cubeEngine = new Module.CubeController(n);
        console.log(`Created ${n}x${n}x${n} Cube!`);
        
        // make sure view animates back before rendering the new cube
        animateToDefault(n, 300, () => {
            const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
            create_cube_from_json(cube_data);
        });

    } catch(e) {
        console.error(e);
        alert("Error initializing cube. Check console.");
    }
}

function applyMove() {
    const move = document.getElementById('moveInput').value;
    if(!cubeEngine || !move) return;
    
    console.log(`> Apply Move: ${move}`);
    
    try {
        cubeEngine.apply_move(move);
        const n = parseInt(document.getElementById('cubeSizeInput').value);
        animateToDefault(n, 300, () => {
            const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
            create_cube_from_json(cube_data);
        });
        document.getElementById('moveInput').value = ''; // clear input
    } catch(e) {
        console.error(e);
    }
}

function scramble() {
    if(!cubeEngine) return;
    console.log(`> Scrambling...`);
    cubeEngine.scramble_cube();
    const n = parseInt(document.getElementById('cubeSizeInput').value);
    animateToDefault(n, 300, () => {
        const cube_data = JSON.parse(cubeEngine.get_cube_state_JSON());
        create_cube_from_json(cube_data);
    });
}

function solve() {
    if(!cubeEngine) return;
    const algo = document.getElementById('solverSelect').value;
    console.log(`> Solving with ${algo}...`);
    // TODO: Add cubeEngine.solve(algo) call when implemented in C++
    alert(`Solving with ${algo} is not implemented yet.`);
}
