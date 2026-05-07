# Rubik's Cube Frontend Documentation

This document explains all functions in the frontend JavaScript code. The frontend uses **Three.js** (a 3D graphics library) to render and animate the cube, and **WebAssembly** (compiled C++ code) to handle cube logic.

---

## TABLE OF CONTENTS

1. [script.js](#scriptjs) - Application Logic & Event Handling
2. [engine.js](#enginejs) - 3D Rendering & Animation
3. [Key Concepts](#key-concepts)

---

## script.js

### Module (C++ to JavaScript Bridge)

```javascript
var Module = {
    print: function(text) { ... },
    printErr: function(text) { ... }
};
```

**What it does:** Redirects output from C++ code (compiled to WebAssembly) to the browser console.

- `print()` - Captures C++ `cout` and displays it in console.log
- `printErr()` - Captures C++ `cerr` and displays it in console.error

---

### startApp()

**What it does:** Initializes the entire application after the user clicks "Start" button.

**Steps:**

1. Hides the entry page (start screen)
2. Shows the main page (3D cube view)
3. Initializes the 3D cube renderer
4. Creates a new cube from C++
5. Sets up all event listeners (buttons, keyboard input)

**Event listeners attached:**

- Scramble button → `scramble()`
- Reset button → `resetCube()`
- Apply Move button → `applyMove()`
- Reset Camera button → `resetCamera()`
- Enter key in move input → `applyMove()`

---

### animateToDefault(n, duration, cb)

**What it does:** Smoothly animates the camera and cube back to the default starting view.

**Parameters:**

- `n` - Size of the cube (e.g., 3 for 3x3x3)
- `duration` - How long the animation should take in milliseconds (default: 500ms)
- `cb` - Callback function to run when animation finishes (optional)

**How it works:**

1. Saves the current camera position and cube rotation
2. Calculates the default camera position based on cube size
3. Uses `requestAnimationFrame` to smoothly interpolate (blend) between current and default states
4. When animation finishes, calls the callback function

**Key concepts:**

- `lerp` = Linear interpolation (smoothly blend between two positions)
- `slerpQuaternions` = Spherical linear interpolation (smoothly blend rotations)

---

### resetCamera()

**What it does:** Resets the camera view to the default angle.

**How it works:** Calls `animateToDefault()` with a 300ms animation duration but no callback.

---

### initCube()

**What it does:** Creates a new cube and initializes it for play.

**Steps:**

1. Gets the cube size from the input field
2. Validates that the size is 2 or larger
3. Deletes the old cube if one exists
4. Creates a new `CubeController` object from C++ code
5. Animates the view back to default
6. Fetches the cube state from C++ as JSON
7. Renders the cube in 3D using `create_cube_from_json()`

---

### applyMove()

**What it does:** Reads a move notation from the input field, animates it, and updates the cube.

**How moves work:**

1. Gets move string from input field (e.g., "R U R' U'")
2. Splits the string into individual moves
3. For each move:
   - Gets animation information from C++ (which layers rotate, how many times)
   - Animates the rotation on screen using `animateSequence()`
   - Updates the internal cube state in C++
   - Re-renders the cube

**Timeline:**

- Each move takes 500ms to animate
- Moves are executed sequentially (one after another)

---

### scramble()

**What it does:** Randomly shuffles the cube.

**Steps:**

1. Calls `cubeEngine.scramble_cube()` (C++ function)
2. Animates the view back to default
3. Fetches the scrambled cube state
4. Re-renders the cube in 3D

---

### solve()

**What it does:** Attempts to solve the cube (currently not implemented).

**Current behavior:** Shows an alert that solving is not yet implemented.

---

## engine.js

### init_3d_cube(container)

**What it does:** Sets up the entire Three.js 3D graphics engine and initializes the scene.

**Parameters:**

- `container` - The HTML element where the 3D scene will be rendered

**What gets initialized:**

1. **Scene** - The 3D world where objects live
2. **Camera** - The viewpoint into the 3D world
3. **Renderer** - The engine that draws everything to the screen
4. **cubeParent** - A group that holds the cube (used for rotating the whole cube)

**Features set up:**

- Mouse drag handling (click and drag to rotate the view)
- Inertia rotation (cube keeps spinning after you release the mouse)
- Window resize handling (adjusts 3D view when window resizes)
- Animation loop (continuous rendering)

---

### onMouseMove(e)

**What it does:** Handles mouse movement while dragging.

**How it works:**

1. Calculates how far the mouse moved since last frame (deltaMove)
2. Creates two rotation quaternions:
   - Horizontal rotation (around the Y-axis/up direction)
   - Vertical rotation (around the camera's right direction)
3. Combines these rotations into `inertiaRotation`
4. Applies the rotation to the cube

**Key concept:** `Quaternion` = A way to represent 3D rotations that works better than Euler angles (no gimbal lock).

---

### onMouseUp()

**What it does:** Stops tracking mouse movement when the user releases the mouse button.

**Steps:**

1. Sets `isDragging` to false
2. Removes the mousemove and mouseup event listeners

---

### Resize Handler

**What it does:** Adjusts the 3D view when the user resizes their browser window.

**Steps:**

1. Updates the camera's aspect ratio
2. Updates the renderer size to match the container

---

### Animation Loop (animate function)

**What it does:** Continuously renders the 3D scene 60 times per second.

**Steps each frame:**

1. Applies inertia rotation if the cube is not being dragged
2. Dampens (slows down) the inertia rotation each frame
3. Renders the scene with the current camera view

**Inertia effect:** The cube keeps spinning after you release the mouse, gradually slowing down.

---

### rotateAroundPoint(obj, point, axis, angle)

**What it does:** Rotates a 3D object around a specific point in space (like spinning a layer around the cube's center).

**Parameters:**

- `obj` - The 3D object to rotate
- `point` - The center point to rotate around (world coordinates)
- `axis` - The axis to rotate around (X, Y, or Z)
- `angle` - How much to rotate in radians

**How it works:**

1. Moves the object so the rotation point is at the origin
2. Applies the rotation
3. Moves the object back to its correct position

**Why this is needed:** When you rotate a cube face (e.g., right face), the cubies on that face rotate around the cube's center, not their own centers.

---

### pivotForLayer(axisVec, layerIndex, n)

**What it does:** Calculates the center point around which a specific layer rotates.

**Parameters:**

- `axisVec` - The rotation axis (e.g., X, Y, or Z)
- `layerIndex` - Which layer to rotate (0, 1, 2, etc.)
- `n` - Total cube size

**Returns:** A 3D point (Vector3) that represents the rotation center.

**Example:** For a 3x3 cube, the right face (layerIndex=2, axis=X) rotates around the point (1.05, 0, 0).

---

### animateLayerRotation(layerIndex, axisVec, angle, duration, cb)

**What it does:** Animates the rotation of a single layer over time.

**Parameters:**

- `layerIndex` - Which layer to rotate
- `axisVec` - The rotation axis
- `angle` - Total rotation amount in radians
- `duration` - How long the animation should take in milliseconds
- `cb` - Callback function when animation finishes

**How it works:**

1. Finds all cubies (small cubes) in that layer
2. Uses `requestAnimationFrame` to animate smoothly
3. Each frame, rotates the cubies a bit more around the pivot point
4. When complete, calls the callback

---

### axisToString(axisVec)

**What it does:** Converts a 3D axis vector to a letter.

**Conversion:**

- (1,0,0) → 'x'
- (0,1,0) → 'y'
- (0,0,1) → 'z'

**Used for:** Looking up which coordinate changed (e.g., "which cubies have the same x value?")

---

### axisStringToVector(str)

**What it does:** Converts a letter to a 3D axis vector.

**Conversion:**

- 'X' → (1,0,0)
- 'Y' → (0,1,0)
- 'Z' → (0,0,1)

**Used for:** Converting move notation (R, U, F moves) into 3D rotation axes.

---

### getCurrentCubeSize()

**What it does:** Determines the cube size by looking at the coordinates of all cubies.

**How it works:** Finds the maximum coordinate value + 1.

**Example:** If the highest coordinate is 2, then the cube is 3x3x3.

---

### remapCoordsForItem(item)

**What it does:** Updates the internal coordinates of all cubies after a move is animated.

**Why it's needed:** When you rotate the right face, the cubies' coordinates need to change. This function calculates the new coordinates based on the rotation.

**Parameters:**

- `item` - Object containing:
  - `axis` - Which axis was rotated (X, Y, Z)
  - `layer` - Which layer(s) rotated
  - `type` - 'face' for single layer, 'wide' for multiple layers
  - `num_layers` - How many layers rotated (for wide moves)
  - `rotations` - How many 90° rotations (1, 2, or 3)

**How it works:**

1. Determines which layers were affected
2. For each cubie on those layers, recalculates its coordinates
3. Uses rotation math to figure out the new position

**Example:** If you rotate the X-axis by 90°:

- Y coordinate → Z coordinate
- Z coordinate → -Y coordinate

---

### animateSequence(items, duration, cb)

**What it does:** Animates a sequence of moves one after another.

**Parameters:**

- `items` - Array of move objects to animate
- `duration` - How long each move should take
- `cb` - Callback when all moves are done

**How it works:**

1. Takes the first move from the array
2. Animates it using `animateMoveItem()`
3. After animation finishes, updates coordinates with `remapCoordsForItem()`
4. Recursively calls itself with the remaining moves
5. When no moves left, calls the callback

---

### animateLayersTogether(layers, axisVec, angle, duration, cb)

**What it does:** Animates multiple layers rotating simultaneously (for "wide" moves like Rw).

**Parameters:**

- `layers` - Array of layer indices to rotate together
- `axisVec` - The rotation axis
- `angle` - Total rotation in radians
- `duration` - Animation duration in milliseconds
- `cb` - Callback when done

**How it works:**

1. Prepares rotation centers (pivots) for each layer
2. Prepares lists of cubies for each layer
3. Each animation frame, rotates all layers by the same amount
4. All layers finish at the same time

**Difference from sequential:** Sequential waits for layer 1 to finish, then rotates layer 2. Simultaneous rotates all layers at the same time.

---

### animateMoveItem(item, duration, cb)

**What it does:** Animates a single move (which may affect one or more layers).

**Parameters:**

- `item` - Object describing the move (axis, layers, rotations, etc.)
- `duration` - How long to animate in milliseconds
- `cb` - Callback when done

**How it works:**

1. Determines which layers are affected
2. Calculates the rotation direction based on camera position
   - If you're looking at the right side of the cube, rotating right should go clockwise
   - If you're looking from the left, it should go counter-clockwise
3. Converts rotation count to radians (1 = 90°, 2 = 180°, 3 = 270°)
4. Either:
   - Animates multiple layers simultaneously (if `simultaneous` flag is set)
   - Animates layers sequentially (one after another)

---

### resetView(n)

**What it does:** Resets the cube to its default position and orientation without animation.

**Parameters:**

- `n` - Cube size

**What it resets:**

1. Cube parent rotation (quaternion) to identity (no rotation)
2. Inertia rotation to zero
3. Camera position based on cube size

**Used when:** Loading a new cube, to ensure it starts at the correct angle.

---

### create_cube_from_json(cube_data)

**What it does:** Creates all the visual cubies from cube state data and renders them in 3D.

**Parameters:**

- `cube_data` - JSON object from C++ containing:
  - `size` - Cube dimensions (n for nxnxn)
  - `cubies` - Array of cubie objects with:
    - `colors` - String like "RWGYBO" (colors on each face)
    - `pos` - Object with x, y, z coordinates

**Steps:**

1. Removes old cube if it exists
2. Resets the view to default
3. For each cubie in the data:
   - Creates a 3D box (geometry)
   - Creates materials (colors) for each face
   - Positions it in 3D space
   - Stores its coordinates for later animation
   - Adds it to the cube group
4. Adds the complete cube to the scene

**Skips internal cubies:** Cubies with colors="XXXXXX" are not rendered (they're not visible inside the cube).

---

### create_cubie_materials(colors)

**What it does:** Creates the visual materials (colors) for the six faces of a cubie.

**Parameters:**

- `colors` - A 6-character string like "RWGYBO" where each character represents:
  - W = White
  - R = Red
  - Y = Yellow
  - O = Orange
  - G = Green
  - B = Blue
  - X = Black/hidden (internal face)

**Returns:** An array of 6 THREE.MeshBasicMaterial objects (one per face).

**Face order:** Right, Left, Up, Down, Front, Back

---

## Key Concepts

### Three.js Basics

**Scene:** The 3D world. Everything lives in the scene.

**Camera:** The viewpoint. You look at the scene through the camera.

**Renderer:** Converts the scene and camera into pixels on the screen.

**Mesh:** A 3D object made of geometry (shape) and material (appearance).

**Group:** A container that holds multiple objects. Rotating a group rotates all objects inside it.

### Quaternions

A **quaternion** is a way to represent 3D rotations using 4 numbers (x, y, z, w). They're better than Euler angles because they avoid "gimbal lock" (loss of rotation freedom).

**Key quaternion operations:**

- `premultiply()` - Combine rotations in local space
- `slerp()` - Smoothly blend between two rotations (Spherical Linear Interpolation)
- `slerpQuaternions()` - Blend between two quaternions

### Vectors

A **Vector3** represents a 3D point or direction with three coordinates (x, y, z).

**Common operations:**

- `sub()` - Subtract (move relative to another point)
- `add()` - Add
- `applyAxisAngle()` - Rotate around an axis
- `dot()` - Dot product (how aligned two directions are)

### Coordinates

**Cube space (C++):**

- Cubies stored in 3D grid [layer][row][col]
- X-axis: left(-) to right(+)
- Y-axis: down(-) to up(+)
- Z-axis: back(-) to front(+)

**Display coordinates:**

- Cubies positioned in 3D space with spacing between them
- Calculated as: `coord * (cubie_size + spacing) - offset`

### Animation

**requestAnimationFrame:** Tells the browser to call a function before the next frame is drawn.

**Time-based animation:**

1. Save the start time
2. Each frame, calculate progress: `t = (current_time - start_time) / duration`
3. Interpolate based on progress
4. Stop when `t >= 1`

---

## Workflow: Applying a Move

Here's what happens when you enter "R" and press Enter:

1. `applyMove()` reads "R" from the input
2. It calls C++ function `get_move_animation_info("R")`
3. C++ returns animation data: `{axis: "X", layer: 2, rotations: 1, ...}`
4. `animateSequence()` is called with this data
5. `animateMoveItem()` animates the rotation:
   - Finds all cubies with x=2
   - Rotates them around the cube center
   - Animates over 500ms
6. After animation completes, `remapCoordsForItem()` updates coordinates
7. `cubeEngine.apply_move("R")` updates the internal cube state in C++
8. `get_cube_state_JSON()` gets the new state
9. `create_cube_from_json()` re-renders the cube
10. If more moves exist, loop back to step 5

---

## Troubleshooting Tips

- **Cube not rotating correctly:** Check `animateMoveItem()` - the rotation direction depends on camera position
- **Coordinates out of sync:** Check `remapCoordsForItem()` - it needs to match the C++ rotation logic
- **Camera too close/far:** Adjust the camera position in `resetView()` based on cube size
- **Performance issues:** Reduce animation duration, or limit the number of simultaneous animations
