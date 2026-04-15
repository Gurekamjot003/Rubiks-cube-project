const createModule = require('./Front end/RubiksCube.js');
createModule({}).then(mod => {
    const cube = new mod.CubeController(3);
    console.log('methods:', Object.getOwnPropertyNames(Object.getPrototypeOf(cube)));
    console.log('get_move_animation_info type:', typeof cube.get_move_animation_info);
});