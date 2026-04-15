const Module = require('./Front end/RubiksCube.js');
Module.ready.then(() => {
    const cube = new Module.CubeController(3);
    console.log('prototype methods:', Object.getOwnPropertyNames(Object.getPrototypeOf(cube)));
    console.log('get_move_animation_info type:', typeof cube.get_move_animation_info);
    process.exit(0);
});