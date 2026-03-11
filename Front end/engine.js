
// Three.js and Cube.js will be imported via script tags in index.html

const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera( 75, 1, 0.1, 1000 ); // Initial aspect ratio set to 1, will be updated immediately
const renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });

let rubiksCube;
let cubeParent;
let isDragging = false;
let previousMousePosition = { x: 0, y: 0 };
let inertiaRotation = new THREE.Quaternion();
const damping = 0.95; // Higher value = more inertia

function init_3d_cube(container) {
    renderer.setClearColor(0x000000, 0); // Make renderer background transparent
    container.appendChild( renderer.domElement );

    // Set size and aspect ratio correctly from the start
    renderer.setSize( container.clientWidth, container.clientHeight );
    camera.aspect = container.clientWidth / container.clientHeight;
    camera.updateProjectionMatrix();

    cubeParent = new THREE.Group();
    scene.add(cubeParent);

    camera.position.set(4, 4, 6);
    camera.lookAt(scene.position);

    // --- Mouse Drag Logic ---
    renderer.domElement.addEventListener('mousedown', (e) => {
        isDragging = true;
        previousMousePosition = { x: e.clientX, y: e.clientY };
        inertiaRotation.set(0, 0, 0, 1); // Reset inertia on new drag

        // Attach listeners to the window for continuous movement
        window.addEventListener('mousemove', onMouseMove);
        window.addEventListener('mouseup', onMouseUp);
    });

    function onMouseMove(e) {
        if (!isDragging || !cubeParent) return;

        const deltaMove = {
            x: e.clientX - previousMousePosition.x,
            y: e.clientY - previousMousePosition.y
        };
        previousMousePosition = { x: e.clientX, y: e.clientY };

        const rotationSpeed = 0.005;

        // Rotation axes based on camera view
        const yAxis = new THREE.Vector3(0, 1, 0); // World-up for turntable-style horizontal rotation
        const cameraRight = new THREE.Vector3(1, 0, 0).applyQuaternion(camera.quaternion); // Camera's local right for vertical rotation

        // Create quaternions for horizontal and vertical rotations
        const quatHorizontal = new THREE.Quaternion().setFromAxisAngle(yAxis, deltaMove.x * rotationSpeed);
        const quatVertical = new THREE.Quaternion().setFromAxisAngle(cameraRight, deltaMove.y * rotationSpeed);

        // Combine rotations: horizontal then vertical. This becomes the inertia.
        inertiaRotation.multiplyQuaternions(quatHorizontal, quatVertical);
        
        // Apply the rotation to the cube
        cubeParent.quaternion.premultiply(inertiaRotation);
    }

    function onMouseUp() {
        isDragging = false;
        // Remove listeners from the window
        window.removeEventListener('mousemove', onMouseMove);
        window.removeEventListener('mouseup', onMouseUp);
    }


    // --- Resize Logic ---
    window.addEventListener('resize', () => {
        if (container) {
            camera.aspect = container.clientWidth / container.clientHeight;
            camera.updateProjectionMatrix();
            renderer.setSize(container.clientWidth, container.clientHeight);
        }
    }, false);


    // --- Animation Loop ---
    const animate = function () {
        requestAnimationFrame( animate );

        if (cubeParent && !isDragging) {
            // Apply and dampen inertia rotation
            const identity = new THREE.Quaternion();
            if (!inertiaRotation.equals(identity)) {
                cubeParent.quaternion.premultiply(inertiaRotation);
                inertiaRotation.slerp(identity, 1 - damping);
            }
        }

        renderer.render( scene, camera );
    };

    animate();
}

function resetView(n) {
    // reset the parent group orientation and inertia so the cube returns to default
    if (cubeParent) {
        cubeParent.quaternion.set(0, 0, 0, 1);
    }
    inertiaRotation.set(0, 0, 0, 1);

    // camera adjustment based on size
    if (typeof n === 'number') {
        const distance_factor = 2.0;
        const camera_distance = n * distance_factor;
        camera.position.set(camera_distance, camera_distance * 0.75, camera_distance);
        camera.lookAt(scene.position);
    }
}

function create_cube_from_json(cube_data) {
    if (rubiksCube) {
        cubeParent.remove(rubiksCube);
    }

    const n = cube_data.size;

    // --- Reset view/orientation ---
    resetView(n);


    // --- Cube Creation ---
    const cubie_size = 1;
    const spacing = 0.1;
    const cube_assembly_size = n * cubie_size + (n - 1) * spacing;
    const offset = (cube_assembly_size - cubie_size) / 2;

    rubiksCube = new THREE.Group();

    for (const cubie_data of cube_data.cubies) {
        // Simple check to not render the fully internal cubie
        if (cubie_data.colors === 'XXXXXX') continue;

        const geometry = new THREE.BoxGeometry(cubie_size, cubie_size, cubie_size);
        const materials = create_cubie_materials(cubie_data.colors);
        const cubie = new THREE.Mesh(geometry, materials);

        // Position cubies relative to the center of the group
        cubie.position.x = cubie_data.pos.x * (cubie_size + spacing) - offset;
        cubie.position.y = cubie_data.pos.y * (cubie_size + spacing) - offset;
        cubie.position.z = cubie_data.pos.z * (cubie_size + spacing) - offset;
        
        // Correct for Three.js's coordinate system if necessary (Y is up)
        // This seems to be handled correctly by the C++ to JS coordinate mapping already.

        rubiksCube.add(cubie);
    }

    cubeParent.add(rubiksCube);
}

function create_cubie_materials(colors) {
    const color_map = {
        'W': 0xffffff, // White
        'G': 0x00ff00, // Green
        'R': 0xff0000, // Red
        'B': 0x0000ff, // Blue
        'O': 0xffa500, // Orange
        'Y': 0xffff00, // Yellow
        'X': 0x333333, // Black/Gray for internal faces
    };

    const face_order = ['R', 'L', 'U', 'D', 'F', 'B'];

    return face_order.map(face => {
        const color_char = colors[face_order.indexOf(face)];
        return new THREE.MeshBasicMaterial({ 
            color: color_map[color_char] || 0x000000,
            side: THREE.FrontSide 
        });
    });
}
