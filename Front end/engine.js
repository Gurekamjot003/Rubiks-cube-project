
// ==================== THREE.JS 3D GRAPHICS SETUP ====================
// Three.js is a popular JavaScript library for rendering 3D graphics in the browser

// Create the 3D scene - this is the container for all 3D objects
const scene = new THREE.Scene();

// Create the camera - this is the viewpoint into the 3D world
// 75 = field of view (degrees), larger = wider view
// 1 = aspect ratio (will be updated to match window size)
// 0.1 = near clipping plane (don't render objects closer than this)
// 1000 = far clipping plane (don't render objects further than this)
const camera = new THREE.PerspectiveCamera( 75, 1, 0.1, 1000 );

// Create the WebGL renderer - this converts the 3D scene into pixels on screen
// antialias: true = smooth edges (looks better)
// alpha: true = transparent background
const renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });

// ==================== GLOBAL VARIABLES ====================
// The complete cube (group containing all cubies/small cubes)
let rubiksCube;

// Parent group that holds the cube - used for rotating the entire cube at once
let cubeParent;

// Mouse interaction state
let isDragging = false;
let previousMousePosition = { x: 0, y: 0 };

// Inertia rotation - makes the cube keep spinning after you release the mouse
// This is a quaternion (4D representation of 3D rotation)
let inertiaRotation = new THREE.Quaternion();

// Damping factor - controls how quickly inertia decays
// 0.95 = 95% of rotation stays each frame (high inertia)
// 0.5 = 50% of rotation stays each frame (quick stop)
const damping = 0.95;

// ==================== 3D ENGINE INITIALIZATION ====================
// Sets up the entire Three.js scene, camera, renderer, and interaction handlers
// This function is called once when the app starts
//
// Parameters:
//   container - The HTML element where the 3D canvas will be placed
function init_3d_cube(container) {
    // Set the background color to black and make it transparent
    // 0x000000 = black in hexadecimal
    // 0 = fully opaque (alpha value 0-1, though not visible behind black anyway)
    renderer.setClearColor(0x000000, 0);

    // Add the 3D canvas element to the HTML container
    container.appendChild( renderer.domElement );

    // ==================== SET CANVAS SIZE ====================
    // Set the renderer size to match the container size
    renderer.setSize( container.clientWidth, container.clientHeight );

    // Update camera aspect ratio to match window dimensions
    // This prevents the image from appearing stretched
    camera.aspect = container.clientWidth / container.clientHeight;
    camera.updateProjectionMatrix();

    // ==================== CREATE SCENE HIERARCHY ====================
    // Create a group to hold the cube
    // This group allows us to rotate the entire cube without affecting individual cubies
    cubeParent = new THREE.Group();
    scene.add(cubeParent);

    // ==================== POSITION CAMERA ====================
    // Set the camera to look at the cube from a diagonal angle
    // This gives a nice isometric view of the cube
    camera.position.set(4, 4, 6);
    // Make the camera look at the center of the scene
    camera.lookAt(scene.position);

    // ==================== MOUSE DRAG INTERACTION ====================
    // When the user presses the mouse button on the canvas
    renderer.domElement.addEventListener('mousedown', (e) => {
        isDragging = true;
        // Record the starting mouse position
        previousMousePosition = { x: e.clientX, y: e.clientY };
        // Reset inertia when starting a new drag
        inertiaRotation.set(0, 0, 0, 1);

        // Attach mouse move/up listeners to the entire window
        // This is important because the mouse might move outside the canvas
        window.addEventListener('mousemove', onMouseMove);
        window.addEventListener('mouseup', onMouseUp);
    });

    // Called while the user is dragging the mouse
    function onMouseMove(e) {
        if (!isDragging || !cubeParent) return;

        // Calculate how far the mouse moved since last frame
        const deltaMove = {
            x: e.clientX - previousMousePosition.x,
            y: e.clientY - previousMousePosition.y
        };
        // Update the previous position for next frame
        previousMousePosition = { x: e.clientX, y: e.clientY };

        // Controls how sensitive the rotation is to mouse movement
        // Smaller = less sensitive, larger = more sensitive
        const rotationSpeed = 0.005;

        // ==================== CALCULATE ROTATION AXES ====================
        // World-up vector for turntable-style horizontal rotation
        // This always points up, regardless of camera orientation
        const yAxis = new THREE.Vector3(0, 1, 0);

        // Camera's local right direction for vertical rotation
        // This vector points to the camera's right, accounting for camera rotation
        const cameraRight = new THREE.Vector3(1, 0, 0).applyQuaternion(camera.quaternion);

        // ==================== CREATE ROTATION QUATERNIONS ====================
        // Create a rotation around the Y-axis based on horizontal mouse movement
        const quatHorizontal = new THREE.Quaternion().setFromAxisAngle(yAxis, deltaMove.x * rotationSpeed);

        // Create a rotation around the camera's right axis based on vertical mouse movement
        const quatVertical = new THREE.Quaternion().setFromAxisAngle(cameraRight, deltaMove.y * rotationSpeed);

        // Combine the two rotations (horizontal then vertical)
        // This rotation will be applied every frame as "inertia" even after mouse release
        inertiaRotation.multiplyQuaternions(quatHorizontal, quatVertical);

        // Apply the rotation to the cube parent immediately
        // premultiply = apply rotation in local space (relative to object's current orientation)
        cubeParent.quaternion.premultiply(inertiaRotation);
    }

    // Called when the user releases the mouse button
    function onMouseUp() {
        isDragging = false;
        // Remove the event listeners - we don't need to track movement anymore
        window.removeEventListener('mousemove', onMouseMove);
        window.removeEventListener('mouseup', onMouseUp);
        // The inertia will continue to be applied in the animation loop
    }


    // ==================== WINDOW RESIZE HANDLING ====================
    // When the user resizes the browser window
    window.addEventListener('resize', () => {
        if (container) {
            // Update camera aspect ratio to match new window size
            camera.aspect = container.clientWidth / container.clientHeight;
            camera.updateProjectionMatrix();
            // Update the renderer to draw at the new resolution
            renderer.setSize(container.clientWidth, container.clientHeight);
        }
    }, false);


    // ==================== ANIMATION LOOP ====================
    // This function is called approximately 60 times per second (60 FPS)
    const animate = function () {
        // Schedule the next animation frame
        requestAnimationFrame( animate );

        // Apply inertia rotation if the user is not dragging
        if (cubeParent && !isDragging) {
            // Create an identity quaternion (no rotation)
            const identity = new THREE.Quaternion();

            // Check if inertia rotation is not zero
            if (!inertiaRotation.equals(identity)) {
                // Apply the current inertia rotation to the cube
                cubeParent.quaternion.premultiply(inertiaRotation);

                // Dampen the inertia rotation towards identity
                // slerp = spherical linear interpolation (smooth blend between rotations)
                // (1 - damping) = amount to dampen each frame
                // 0.95 damping = dampen by 5% each frame = slow decay
                inertiaRotation.slerp(identity, 1 - damping);
            }
        }

        // Draw the scene
        // This actually renders the 3D scene to the screen
        renderer.render( scene, camera );
    };

    // Start the animation loop
    animate();
}



// ==================== ROTATION UTILITIES ====================
// Rotates a 3D object around a specific point in space
// This is used to rotate individual layers of the cube around its center
//
// Parameters:
//   obj - The 3D object to rotate (e.g., a cubie)
//   point - The center point to rotate around (in world coordinates)
//   axis - The axis to rotate around (e.g., X, Y, or Z as a Vector3)
//   angle - How much to rotate in radians (e.g., Math.PI/2 = 90 degrees)
//
// Why this is needed: When rotating a cube face, the cubies must spin around
// the cube's center, not their own centers. This function handles that.
function rotateAroundPoint(obj, point, axis, angle) {
    // Move the object so the rotation point is at the origin (0,0,0)
    obj.position.sub(point);

    // Apply the rotation around the axis
    obj.position.applyAxisAngle(axis, angle);

    // Move the object back to account for the rotation point being offset
    obj.position.add(point);

    // Also rotate the object itself (its orientation)
    const q = new THREE.Quaternion().setFromAxisAngle(axis, angle);
    obj.quaternion.premultiply(q);
}

// ==================== LAYER ROTATION HELPERS ====================
// Calculates the center point around which a specific layer rotates
//
// Parameters:
//   axisVec - The rotation axis as a Vector3 (e.g., (1,0,0) for X-axis)
//   layerIndex - Which layer to rotate (0, 1, 2, etc.)
//   n - Total cube size (e.g., 3 for 3x3x3 cube)
//
// Returns: A 3D point (Vector3) that represents the center of rotation
//
// Example: For a 3x3 cube, rotating the right face (X-axis, layer 2)
// returns the point (1.05, 0, 0) which is the right side of the cube center
function pivotForLayer(axisVec, layerIndex, n) {
    // Calculate the offset from center to the first layer
    // This accounts for the fact that cubies have size and spacing between them
    const offset = (n * CUBIE_SIZE + (n - 1) * SPACING - CUBIE_SIZE) / 2;

    // Calculate the coordinate of this layer
    // Layers are spaced by CUBIE_SIZE + SPACING
    const coord = layerIndex * (CUBIE_SIZE + SPACING) - offset;

    // Create the pivot point at the center of the rotation axis
    const pivot = new THREE.Vector3(0, 0, 0);

    // Set the coordinate on the appropriate axis
    if (axisVec.x === 1) pivot.x = coord;  // X-axis rotation
    if (axisVec.y === 1) pivot.y = coord;  // Y-axis rotation
    if (axisVec.z === 1) pivot.z = coord;  // Z-axis rotation

    return pivot;
}

// Animates the rotation of a single layer over time
//
// Parameters:
//   layerIndex - Which layer to rotate
//   axisVec - The axis to rotate around (e.g., (1,0,0) for X)
//   angle - Total rotation in radians (e.g., Math.PI/2 for 90 degrees)
//   duration - How long the animation should take in milliseconds
//   cb - Callback function to call when animation finishes
//
// How it works: Uses requestAnimationFrame to smoothly rotate the layer
// over the specified duration. Each frame, it calculates the rotation
// progress and rotates the layer that amount.
function animateLayerRotation(layerIndex, axisVec, angle, duration, cb) {
    const n = getCurrentCubeSize();
    // Calculate the point around which this layer rotates
    const pivot = pivotForLayer(axisVec, layerIndex, n);

    // Find all cubies that are in this layer
    // Filter by checking if their coordinate matches the layer index
    const cubes = rubiksCube.children.filter(c =>
        c.userData.coords && c.userData.coords[axisToString(axisVec)] === layerIndex
    );

    let prevAngle = 0;
    const startTime = performance.now();

    // This function is called every animation frame
    function step(time) {
        // Calculate animation progress: 0 = start, 1 = complete
        const t = Math.min(1, (time - startTime) / duration);

        // Calculate how far we should have rotated by now
        const current = angle * t;

        // Calculate the rotation since the last frame
        const delta = current - prevAngle;

        // Rotate each cubie in this layer by the delta amount
        cubes.forEach(cubie => rotateAroundPoint(cubie, pivot, axisVec, delta));

        // Remember this angle for next frame
        prevAngle = current;

        // If animation is not complete, schedule the next frame
        if (t < 1) requestAnimationFrame(step);
        // Animation is complete, call the callback
        else if (cb) cb();
    }

    // Start the animation
    requestAnimationFrame(step);
}

// ==================== AXIS/STRING CONVERSION ====================
// Converts a 3D axis vector to a single character string
//
// Parameters:
//   axisVec - A Vector3 like (1,0,0), (0,1,0), or (0,0,1)
//
// Returns: A single character: 'x', 'y', or 'z'
//
// Used for: Accessing cubie coordinates, e.g., coords['x'], coords['y'], coords['z']
function axisToString(axisVec) {
    if (axisVec.x === 1) return 'x';
    if (axisVec.y === 1) return 'y';
    if (axisVec.z === 1) return 'z';
    return '';
}

// Converts a single character string to a 3D axis vector
//
// Parameters:
//   str - A single character: 'X', 'Y', or 'Z'
//
// Returns: A Vector3 representing that axis
//   'X' → (1, 0, 0)
//   'Y' → (0, 1, 0)
//   'Z' → (0, 0, 1)
//
// Used for: Converting move notation (R, U, F) into 3D rotation axes
function axisStringToVector(str) {
    if (str === 'X') return new THREE.Vector3(1, 0, 0);
    if (str === 'Y') return new THREE.Vector3(0, 1, 0);
    if (str === 'Z') return new THREE.Vector3(0, 0, 1);
    return new THREE.Vector3();
}

// Determines the cube size by examining the cubies' coordinates
//
// Returns: The cube size (n for an n×n×n cube)
//
// How it works: Finds the maximum coordinate value among all cubies
// and adds 1. For example, if the highest coordinate is 2, the cube is 3×3×3.
function getCurrentCubeSize() {
    let max = 0;
    // Look at all cubies and find the highest coordinate
    rubiksCube.children.forEach(c => {
        const coords = c.userData.coords;
        if (coords) {
            max = Math.max(max, coords.x, coords.y, coords.z);
        }
    });
    // Size is max coordinate + 1
    return max + 1;
}


// ==================== COORDINATE REMAPPING ====================
// Updates the internal coordinates of cubies after a move is animated
//
// Why this is needed: When you visually rotate a layer, the cubies' positions
// in the 3D scene change, but their logical grid coordinates also need to update.
// This function calculates the new coordinates based on the rotation.
//
// Parameters:
//   item - An object describing the move with properties:
//     - axis: The rotation axis as a string ('X', 'Y', or 'Z')
//     - layer: Which layer was rotated
//     - type: Either 'face' (single layer) or 'wide' (multiple layers)
//     - num_layers: How many layers were affected (for wide moves)
//     - rotations: How many 90° rotations (1, 2, or 3)
//
// How it works:
// 1. Determines which layers were affected by this move
// 2. For each cubie on those layers, recalculates its coordinates
// 3. Uses coordinate transformation math based on the rotation axis and direction
//
// Example: If rotating around X-axis (layer of right face):
// - Y coordinate becomes Z coordinate
// - Z coordinate becomes negative Y coordinate
// - This represents a 90° clockwise rotation when viewed from the right side
function remapCoordsForItem(item) {
    // Convert the axis string to a Vector3
    const axisVec = axisStringToVector(item.axis);

    // Determine which layers were affected by this move
    let layers = [];
    if (item.type === 'face') {
        // Single layer move (e.g., R, U, F)
        layers.push(item.layer);
    } else if (item.type === 'wide') {
        // Wide move affects multiple layers (e.g., Rw, Uw)
        // Calculate which direction to expand (towards center or away)
        const d = (item.layer === 0 ? 1 : -1);
        for (let i = 0; i < item.num_layers; i++) {
            layers.push(item.layer + i * d);
        }
    }

    // Normalize the rotation count
    // 3 rotations = -1 rotation (same as rotating backwards once)
    let count = item.rotations;
    if (count === 3) count = -1;

    // Update coordinates for all cubies
    rubiksCube.children.forEach(c => {
        const coords = c.userData.coords;
        if (!coords) return;

        // ==================== X-AXIS ROTATION ====================
        // When rotating around X-axis (left/right), Y and Z coordinates change
        if (axisVec.x === 1 && layers.includes(coords.x)) {
            // Store original coordinates
            let y = coords.y, z = coords.z;

            // Apply the rotation transformation
            // Each 90° rotation: [y, z] → [-z, y] (clockwise when viewed from right)
            for (let i = 0; i < Math.abs(count); i++) {
                if (count > 0) [y, z] = [-z, y];     // Clockwise
                else [y, z] = [z, -y];               // Counter-clockwise
            }

            // Update the cubie's coordinates
            coords.y = y;
            coords.z = z;
        }
        // ==================== Y-AXIS ROTATION ====================
        // When rotating around Y-axis (up/down), X and Z coordinates change
        else if (axisVec.y === 1 && layers.includes(coords.y)) {
            let x = coords.x, z = coords.z;

            // Each 90° rotation: [x, z] → [z, -x]
            for (let i = 0; i < Math.abs(count); i++) {
                if (count > 0) [x, z] = [z, -x];     // Clockwise
                else [x, z] = [-z, x];               // Counter-clockwise
            }

            coords.x = x;
            coords.z = z;
        }
        // ==================== Z-AXIS ROTATION ====================
        // When rotating around Z-axis (front/back), X and Y coordinates change
        else if (axisVec.z === 1 && layers.includes(coords.z)) {
            let x = coords.x, y = coords.y;

            // Each 90° rotation: [x, y] → [-y, x]
            for (let i = 0; i < Math.abs(count); i++) {
                if (count > 0) [x, y] = [-y, x];     // Clockwise
                else [x, y] = [y, -x];               // Counter-clockwise
            }

            coords.x = x;
            coords.y = y;
        }
    });
}

// ==================== ANIMATION SEQUENCING ====================
// Animates a sequence of moves one after another
//
// Parameters:
//   items - Array of move objects to animate sequentially
//   duration - How long each move should take in milliseconds
//   cb - Callback function to call when all moves are complete
//
// How it works:
// 1. Takes the first move from the array
// 2. Animates it using animateMoveItem()
// 3. After animation finishes, remaps coordinates
// 4. Recursively calls itself with remaining moves
// 5. When no moves left, calls the callback
//
// This ensures moves happen one at a time, not all at once
function animateSequence(items, duration, cb) {
    // Base case: no more items to animate
    if (!items || items.length === 0) {
        if (cb) cb();
        return;
    }

    // Remove and animate the first item
    const item = items.shift();

    // Animate this move
    animateMoveItem(item, duration, () => {
        // After animation, update the logical coordinates
        remapCoordsForItem(item);

        // Recursively animate the rest of the moves
        animateSequence(items, duration, cb);
    });
}

// ==================== ANIMATION VARIATIONS ====================
// Animates multiple layers rotating simultaneously at the same speed
// Used for "wide" moves like Rw (rotate right two layers together)
//
// Parameters:
//   layers - Array of layer indices to rotate
//   axisVec - The rotation axis (e.g., (1,0,0) for X)
//   angle - Total rotation in radians
//   duration - How long the animation should take in milliseconds
//   cb - Callback when done
//
// Difference from sequential: All layers finish at the same time, rotating together
// Instead of: layer 1 finishes, then layer 2 starts
function animateLayersTogether(layers, axisVec, angle, duration, cb) {
    const n = getCurrentCubeSize();

    // Prepare the rotation center for each layer
    const pivots = layers.map(l => pivotForLayer(axisVec, l, n));

    // For each layer, collect all cubies that need to rotate
    const cubesPerLayer = layers.map(l =>
        rubiksCube.children.filter(c =>
            c.userData.coords && c.userData.coords[axisToString(axisVec)] === l
        )
    );

    let prevAngle = 0;
    const startTime = performance.now();

    // Animation frame callback
    function step(time) {
        // Calculate animation progress (0 to 1)
        const t = Math.min(1, (time - startTime) / duration);

        // Calculate total rotation so far
        const current = angle * t;

        // Calculate rotation since last frame
        const delta = current - prevAngle;

        // Rotate each layer
        layers.forEach((l, i) => {
            const pivot = pivots[i];
            cubesPerLayer[i].forEach(cubie =>
                rotateAroundPoint(cubie, pivot, axisVec, delta)
            );
        });

        prevAngle = current;

        // Continue if not done
        if (t < 1) requestAnimationFrame(step);
        else if (cb) cb();
    }

    requestAnimationFrame(step);
}

// ==================== MAIN MOVE ANIMATION ====================
// Animates a single move (which may affect one or more layers)
//
// Parameters:
//   item - Object containing:
//     - axis: Rotation axis ('X', 'Y', or 'Z')
//     - layer: Starting layer index
//     - type: 'face' (single layer) or 'wide' (multiple layers)
//     - num_layers: How many layers affected (for wide moves)
//     - rotations: How many 90° rotations (1, 2, or 3)
//     - simultaneous: Whether to animate layers together (for wide moves)
//   duration - How long the animation should take in milliseconds
//   cb - Callback when animation finishes
//
// Complex logic:
// 1. Determines which layers are affected
// 2. Figures out rotation direction based on camera viewpoint
//   - Ensures moves appear correct from the user's perspective
//   - If looking at the right side, right face rotates clockwise
// 3. Converts rotation count to radians (1=90°, 2=180°, 3=270°)
// 4. Either animates all layers together (simultaneous) or sequentially
function animateMoveItem(item, duration, cb) {
    const n = getCurrentCubeSize();
    const axisVec = axisStringToVector(item.axis);

    // Determine which layers this move affects
    let layers = [];
    if (item.type === 'face') {
        // Single layer (e.g., R, U, F)
        layers.push(item.layer);
    } else if (item.type === 'wide') {
        // Multiple layers (e.g., Rw = R + M)
        // Determine direction: 0 = expand outward, non-0 = expand inward
        const d = (item.layer === 0 ? 1 : -1);
        for (let i = 0; i < item.num_layers; i++) {
            layers.push(item.layer + i * d);
        }
    }

    // ==================== CALCULATE ROTATION DIRECTION ====================
    // The rotation direction depends on the camera viewpoint
    // Example: Rotating the right face:
    //   - If looking from the right: clockwise = positive angle
    //   - If looking from the left: counter-clockwise = positive angle
    //
    // We determine this by checking the dot product of the camera direction
    // and the rotation axis

    // Get the first layer's pivot point
    const firstPivot = pivotForLayer(axisVec, layers[0], n);

    // Calculate camera direction to this pivot
    const cameraDir = camera.position.clone().sub(firstPivot);

    // Check if camera is on the positive or negative side of the axis
    // Positive side = rotate counter-clockwise from camera view = multiply by -1
    // Negative side = rotate clockwise from camera view = multiply by +1
    const sign = cameraDir.dot(axisVec) > 0 ? -1 : 1;

    // Convert rotation count to radians
    // 1 = 90° = π/2, 2 = 180° = π, 3 = 270° = 3π/2
    let count = item.rotations;
    if (count === 3) count = -1;  // 3 rotations = 1 rotation backwards

    // Convert count to angle in radians
    const angle = sign * count * Math.PI / 2;

    // ==================== CHOOSE ANIMATION STRATEGY ====================
    if (item.type === 'wide' && item.simultaneous) {
        // Wide move with simultaneous flag: rotate all layers at the same time
        animateLayersTogether(layers, axisVec, angle, duration, cb);
    } else {
        // Default: rotate layers sequentially (one after another)
        let idx = 0;

        function nextLayer() {
            if (idx >= layers.length) {
                // All layers done
                if (cb) cb();
                return;
            }

            const layerIdx = layers[idx++];
            const pivot = pivotForLayer(axisVec, layerIdx, n);

            // Each layer gets an equal share of the total duration
            const layerDuration = duration / layers.length;

            animateLayerRotation(layerIdx, axisVec, angle, layerDuration, nextLayer);
        }

        nextLayer();
    }
}


// ==================== VIEW MANAGEMENT ====================
// Resets the cube to its default position and camera angle (instantly, without animation)
//
// Parameters:
//   n - Cube size (used to calculate camera distance)
//
// What it resets:
// 1. Cube parent rotation back to identity (no rotation)
// 2. Inertia rotation to zero (cube stops spinning)
// 3. Camera position based on cube size
//
// When used: Loading a new cube, to ensure it starts at a clean state
function resetView(n) {
    // Reset the cube parent's rotation to identity (no rotation)
    if (cubeParent) {
        cubeParent.quaternion.set(0, 0, 0, 1);
    }

    // Reset inertia so the cube stops spinning
    inertiaRotation.set(0, 0, 0, 1);

    // Reposition the camera based on cube size
    if (typeof n === 'number') {
        const distance_factor = 2.0;
        const camera_distance = n * distance_factor;

        // Position the camera diagonally from the cube for a nice isometric view
        camera.position.set(camera_distance, camera_distance * 0.75, camera_distance);

        // Point the camera at the scene center
        camera.lookAt(scene.position);
    }
}

// ==================== CUBE CREATION ====================
// Creates all the visual cubies (small cubes) from JSON data and renders them in 3D
//
// Parameters:
//   cube_data - JSON object from C++ backend containing:
//     - size: Cube dimension (n for n×n×n cube)
//     - cubies: Array of cubie objects, each with:
//       - colors: 6-character string (R, L, U, D, F, B faces)
//       - pos: {x, y, z} - 3D grid coordinates
//
// Process:
// 1. Remove the old cube if one exists
// 2. Reset the view to default
// 3. For each cubie:
//    - Create a 3D box mesh
//    - Create materials for each face with the correct colors
//    - Position it in 3D space
//    - Store its grid coordinates for animation
//    - Add it to the cube group
// 4. Add the complete cube to the 3D scene
function create_cube_from_json(cube_data) {
    // Remove the old cube from the scene if it exists
    if (rubiksCube) {
        cubeParent.remove(rubiksCube);
    }

    // Extract the cube size
    const n = cube_data.size;

    // ==================== RESET VIEW ====================
    resetView(n);

    // ==================== CALCULATE LAYOUT ====================
    // Size of each small cube (in 3D units)
    const cubie_size = 1;

    // Space between cubies
    const spacing = 0.1;

    // Total size of the assembled cube
    const cube_assembly_size = n * cubie_size + (n - 1) * spacing;

    // Offset to center the cube at (0, 0, 0)
    const offset = (cube_assembly_size - cubie_size) / 2;

    // ==================== CREATE CUBE GROUP ====================
    // Create a group to hold all the cubies
    rubiksCube = new THREE.Group();

    // ==================== CREATE EACH CUBIE ====================
    for (const cubie_data of cube_data.cubies) {
        // Skip internal cubies that aren't visible
        // Internal cubies are not on the surface of the cube
        const isInternal = (
            cubie_data.pos.x > 0 && cubie_data.pos.x < cube_data.size - 1 &&
            cubie_data.pos.y > 0 && cubie_data.pos.y < cube_data.size - 1 &&
            cubie_data.pos.z > 0 && cubie_data.pos.z < cube_data.size - 1
        );
        if (isInternal) continue;

        // ==================== CREATE GEOMETRY ====================
        // Create a cube shape (1×1×1)
        const geometry = new THREE.BoxGeometry(cubie_size, cubie_size, cubie_size);

        // Create colored materials for the 6 faces
        const materials = create_cubie_materials(cubie_data.colors);

        // Create the mesh (geometry + materials)
        const cubie = new THREE.Mesh(geometry, materials);

        // ==================== POSITION CUBIE ====================
        // Position the cubie in 3D space based on its grid coordinates
        // We convert grid coordinates to world coordinates, accounting for spacing and centering
        cubie.position.x = cubie_data.pos.x * (cubie_size + spacing) - offset;
        cubie.position.y = cubie_data.pos.y * (cubie_size + spacing) - offset;
        cubie.position.z = cubie_data.pos.z * (cubie_size + spacing) - offset;

        // ==================== STORE GRID COORDINATES ====================
        // Save the grid coordinates for animation logic
        // Later, when animating moves, we need to know which cubies are in each layer
        cubie.userData.coords = {
            x: cubie_data.pos.x,
            y: cubie_data.pos.y,
            z: cubie_data.pos.z
        };

        // Add the cubie to the cube group
        rubiksCube.add(cubie);
    }

    // Add the complete cube to the scene
    cubeParent.add(rubiksCube);
}

// ==================== GEOMETRY CONSTANTS ====================
// Size of each individual small cube
const CUBIE_SIZE = 1;

// Space between cubies
const SPACING = 0.1;

// ==================== MATERIAL CREATION ====================
// Creates the colored materials for the 6 faces of a cubie
//
// Parameters:
//   colors - A 6-character string where each character represents a face color
//     Example: "RWGYBO" means Right=Red, Left=White, Up=Green, Down=Yellow, Front=Blue, Back=Orange
//
// Color mappings:
//   W = White    (0xffffff)
//   G = Green    (0x00ff00)
//   R = Red      (0xff0000)
//   B = Blue     (0x0000ff)
//   O = Orange   (0xffa500)
//   Y = Yellow   (0xffff00)
//   X = Hidden   (0x333333) - internal faces
//   E = Empty    (0xcccccc) - external faces with empty color enum
//
// Returns: Array of 6 THREE.MeshBasicMaterial objects, one per face
//
// Face order: Right, Left, Up, Down, Front, Back
// This order matches Three.js's BoxGeometry face ordering
function create_cubie_materials(colors) {
    const isLightMode = document.body.classList.contains('light-theme');
    const emptyColor = isLightMode ? 0x555555 : 0xcccccc; // Dark gray for light mode, light gray for dark mode

    // Mapping from color character to hex color value
    const color_map = {
        'W': 0xffffff,  // White
        'G': 0x00ff00,  // Green
        'R': 0xff0000,  // Red
        'B': 0x0000ff,  // Blue
        'O': 0xffa500,  // Orange
        'Y': 0xffff00,  // Yellow
        'X': 0x333333,  // Black/Gray for internal faces
        'E': emptyColor, // Light/Dark Gray for empty external faces depending on theme
    };

    // Order of faces in Three.js BoxGeometry
    const face_order = ['R', 'L', 'U', 'D', 'F', 'B'];

    // Create a material for each face
    return face_order.map(face => {
        // Get the color character at this position in the colors string
        const color_char = colors[face_order.indexOf(face)];

        // Create a material with this color
        return new THREE.MeshBasicMaterial({
            color: color_map[color_char] || 0x000000,  // Default to black if unknown
            side: THREE.FrontSide  // Only render the front side of each face
        });
    });
}
