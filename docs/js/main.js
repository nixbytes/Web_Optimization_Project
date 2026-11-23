/**
 * BEGINNER'S GUIDE TO WASM GLUE CODE:
 * 
 * When we compile C to WASM, it doesn't just give us a binary. 
 * Emscripten creates a 'glue' JS file (pizza.js) that handles 
 * loading the .wasm file and mapping C functions to JS.
 * 
 * Module: This is the object created by the WASM loader.
 * ccall: A helper function provided by Emscripten to call C functions.
 *   - Param 1: Name of the C function.
 *   - Param 2: Return type (e.g., 'number' for float/int, 'string' for char*).
 *   - Param 3: Array of arguments.
 */

document.addEventListener('DOMContentLoaded', () => {
    const generateBtn = document.getElementById('generate-btn');
    const nameDiv = document.getElementById('pizza-name');
    const ingredientsDiv = document.getElementById('pizza-ingredients');
    const bgParallax = document.getElementById('bg-parallax');

    // We wrap the logic in a function to ensure the WASM module is loaded
    const runPizzaGen = () => {
        try {
            // 1. Get Random Name from C
            // get_random_pizza_name() returns a char* in C
            const name = Module.ccall('get_random_pizza_name', 'string', [], []);
            nameDiv.innerText = name;

            // 2. Get Random Ingredients from C
            // In C, we pass a buffer. In JS/WASM, the easiest way is often 
            // returning a string or using a shared buffer. 
            // For this project's current C implementation, we need to allocate 
            // a buffer in WASM memory.
            
            const bufferSize = 512;
            const bufferPtr = Module._malloc(bufferSize);
            Module.ccall('get_random_pizza_ingredients', null, ['number'], [bufferPtr]);
            const ingredients = Module.UTF8ToString(bufferPtr);
            Module._free(bufferPtr);
            
            ingredientsDiv.innerText = ingredients.replace(/,/g, ', ');
        } catch (e) {
            console.error("WASM Error:", e);
            nameDiv.innerText = "WASM Error!";
            ingredientsDiv.innerText = "Check console for details.";
        }
    };

    generateBtn.addEventListener('click', runPizzaGen);

    // Parallax Effect
    // Use the C function calculate_parallax(scroll_y, index)
    window.addEventListener('scroll', () => {
        const scrollY = window.scrollY;
        const offset = Module.ccall('calculate_parallax', 'number', ['number', 'number'], [scrollY, 0]);
        
        // Move the background slightly to create a depth effect
        bgParallax.style.transform = `translateY(${offset * 20}px)`;
    });
});
