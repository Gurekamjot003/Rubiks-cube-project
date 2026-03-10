
// Three.js and Cube.js will be imported via script tags in index.html

const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 1000 );
const renderer = new THREE.WebGLRenderer({ antialias: true });

let rubiksCube;
let isDragging = false;
let previousMousePosition = { x: 0, y: 0 };

function init_3d_cube(container) {
    renderer.setSize( container.clientWidth, container.clientHeight );
    container.appendChild( renderer.domElement );

    camera.position.set(4, 4, 6);
    camera.lookAt(scene.position);

    renderer.domElement.addEventListener('mousedown', (e) => {
        isDragging = true;
        previousMousePosition = { x: e.offsetX, y: e.offsetY };
    });

    renderer.domElement.addEventListener('mouseup', () => {
        isDragging = false;
    });

    renderer.domElement.addEventListener('mousemove', (e) => {
        if (!isDragging || !rubiksCube) return;

        const deltaMove = {
            x: e.offsetX - previousMousePosition.x,
            y: e.offsetY - previousMousePosition.y
        };

        const rotateAngleX = deltaMove.y * 0.01;
        const rotateAngleY = deltaMove.x * 0.01;

        rubiksCube.rotation.x += rotateAngleX;
        rubiksCube.rotation.y += rotateAngleY;

        previousMousePosition = { x: e.offsetX, y: e.offsetY };
    });

    const animate = function () {
        requestAnimationFrame( animate );
        renderer.render( scene, camera );
    };

    animate();
}

function create_cube_from_json(cube_data) {
    if (rubiksCube) {
        scene.remove(rubiksCube);
    }

    const n = cube_data.size;
    const cubie_size = 1;
    const spacing = 0.1;
    const cube_size = n * cubie_size + (n - 1) * spacing;
    const offset = (cube_size - cubie_size) / 2;

    rubiksCube = new THREE.Group();

    for (const cubie_data of cube_data.cubies) {
        const geometry = new THREE.BoxGeometry(cubie_size, cubie_size, cubie_size);
        const materials = create_cubie_materials(cubie_data.colors);
        const cubie = new THREE.Mesh(geometry, materials);

        cubie.position.x = cubie_data.pos.x * (cubie_size + spacing) - offset;
        cubie.position.y = cubie_data.pos.y * (cubie_size + spacing) - offset;
        cubie.position.z = cubie_data.pos.z * (cubie_size + spacing) - offset;

        rubiksCube.add(cubie);
    }

    scene.add(rubiksCube);
}

function create_cubie_materials(colors) {
    const color_map = {
        'W': 0xffffff, // White
        'G': 0x00ff00, // Green
        'R': 0xff0000, // Red
        'B': 0x0000ff, // Blue
        'O': 0xffa500, // Orange
        'Y': 0xffff00, // Yellow
        'X': 0x000000, // Black (internal)
    };

    const face_order = ['R', 'L', 'U', 'D', 'F', 'B'];

    return face_order.map(face => {
        const color_char = colors[face_order.indexOf(face)];
        return new THREE.MeshBasicMaterial({ color: color_map[color_char] });
    });
}
