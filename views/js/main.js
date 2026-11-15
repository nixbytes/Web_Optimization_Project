var PIZZA_IMAGE_URL = "https://cdn-icons-png.flaticon.com/512/3595/3595455.png";

// WASM Functions
var wasm_get_name, wasm_get_ingredients, wasm_parallax, wasm_size_ratio, wasm_calc_dx;

// Initialize WASM
if (typeof Module !== 'undefined') {
    Module.onRuntimeInitialized = function() {
        wasm_get_name = Module.cwrap('get_random_pizza_name', 'string', []);
        wasm_get_ingredients = Module.cwrap('get_random_pizza_ingredients', null, ['number']);
        wasm_parallax = Module.cwrap('calculate_parallax', 'number', ['number', 'number']);
        wasm_size_ratio = Module.cwrap('get_pizza_size_ratio', 'number', ['number']);
        wasm_calc_dx = Module.cwrap('calculate_dx', 'number', ['number', 'number', 'number']);
        
        // Initial generation now that WASM is ready
        for (var i = 2; i < 100; i++) {
            var pizzasDiv = document.getElementById('randomPizzas');
            pizzasDiv.appendChild(pizzaElementGenerator(i));
        }
        
        window.performance.mark("mark_end_generating");
        window.performance.measure("measure_pizza_generation", "mark_start_generating", "mark_end_generating");
        var timeToGenerate = window.performance.getEntriesByName("measure_pizza_generation");
        console.log("Time to generate pizzas on load: " + timeToGenerate[0].duration + "ms");
    };
}

// Helper to turn comma-separated WASM string into HTML list
var ingredientItemizer = function(string) {
    return "<li>" + string + "</li>";
};

// Returns a string with random pizza ingredients nested inside <li> tags
var makeRandomPizza = function() {
    if (!wasm_get_ingredients) return "<li>WASM Loading...</li>";
    
    // Allocate a buffer for the C function to write into
    var buffer = Module._malloc(512);
    wasm_get_ingredients(buffer);
    var ingredientsStr = Module.UTF8ToString(buffer);
    Module._free(buffer);
    
    var ingredientsArray = ingredientsStr.split(',');
    var html = "";
    for (var i = 0; i < ingredientsArray.length; i++) {
        if (ingredientsArray[i]) {
            html += ingredientItemizer(ingredientsArray[i]);
        }
    }
    return html;
};

// returns a DOM element for each pizza
var pizzaElementGenerator = function(i) {
    var pizzaContainer,
        pizzaImageContainer,
        pizzaImage,
        pizzaDescriptionContainer,
        pizzaName,
        ul;

    pizzaContainer = document.createElement("div");
    pizzaImageContainer = document.createElement("div");
    pizzaImage = document.createElement("img");
    pizzaDescriptionContainer = document.createElement("div");

    pizzaContainer.classList.add("randomPizzaContainer");
    pizzaContainer.style.width = "33.33%";
    pizzaContainer.style.height = "325px";
    pizzaContainer.id = "pizza" + i;
    pizzaImageContainer.style.width="35%";

    pizzaImage.src = PIZZA_IMAGE_URL;
    pizzaImage.classList.add("img-responsive");
    pizzaImageContainer.appendChild(pizzaImage);
    pizzaContainer.appendChild(pizzaImageContainer);

    pizzaDescriptionContainer.style.width="65%";

    pizzaName = document.createElement("h4");
    pizzaName.innerHTML = wasm_get_name ? wasm_get_name() : "Loading...";
    pizzaDescriptionContainer.appendChild(pizzaName);

    ul = document.createElement("ul");
    ul.innerHTML = makeRandomPizza();
    pizzaDescriptionContainer.appendChild(ul);
    pizzaContainer.appendChild(pizzaDescriptionContainer);

    return pizzaContainer;
};

var resizePizzas = function(size) {
    window.performance.mark("mark_start_resize");

    function changeSliderLabel(size) {
        switch(size) {
            case "1": document.getElementById('pizzaSize').innerHTML = "Small"; return;
            case "2": document.getElementById('pizzaSize').innerHTML = "Medium"; return;
            case "3": document.getElementById('pizzaSize').innerHTML = "Large"; return;
            default: console.log("bug in changeSliderLabel");
        }
    }

    changeSliderLabel(size);

    var pizza_containers = document.getElementsByClassName('randomPizzaContainer');
    if (pizza_containers.length === 0) return;

    var windowWidth = document.getElementById('randomPizzas').offsetWidth;
    var oldWidth = pizza_containers[0].offsetWidth;
    
    // Use WASM for the calculation
    var dx = wasm_calc_dx ? wasm_calc_dx(oldWidth, windowWidth, parseInt(size)) : 0;
    var newwidth = (oldWidth + dx) + 'px';

    function changePizzaSizes(size) {
        for (var i = 0; i < pizza_containers.length; i++) {
            pizza_containers[i].style.width = newwidth;
        }
    }

    changePizzaSizes(size);
    window.performance.mark("mark_end_resize");
    window.performance.measure("measure_pizza_resize", "mark_start_resize", "mark_end_resize");
    var timeToResize = window.performance.getEntriesByName("measure_pizza_resize");
    console.log("Time to resize pizzas: " + timeToResize[0].duration + "ms");
};

window.performance.mark("mark_start_generating");

var frame = 0;

function logAverageFrame(times) {
    var numberOfEntries = times.length;
    var sum = 0;
    for (var i = numberOfEntries - 1; i > numberOfEntries - 11; i--) {
        sum = sum + times[i].duration;
    }
    console.log("Average scripting time to generate last 10 frames: " + sum / 10 + "ms");
}

var last_Scroll_Y = 0;
var ticking = true;

function scroll_New() {
    last_Scroll_Y = window.scrollY;
    request_Tick();
}
function request_Tick() {
    if (!ticking) {
        requestAnimationFrame(updatePositions);
    }
    ticking = true;
}
function updatePositions() {
    ticking = false;
    frame++;
    window.performance.mark("mark_start_frame");

    var items = document.querySelectorAll('.mover');
    for (var i = 0; i < items.length; i++) {
        var phase = wasm_parallax ? wasm_parallax(last_Scroll_Y, i) : Math.sin(last_Scroll_Y / 1250.0 + (i % 5));
        items[i].style.left = items[i].basicLeft + 100 * phase + 'px';
    }

    window.performance.mark("mark_end_frame");
    window.performance.measure("measure_frame_duration", "mark_start_frame", "mark_end_frame");
    if (frame % 10 === 0) {
        var timesToUpdatePosition = window.performance.getEntriesByName("measure_frame_duration");
        logAverageFrame(timesToUpdatePosition);
    }
}

window.addEventListener('scroll', scroll_New);

document.addEventListener('DOMContentLoaded', function() {
    var cols = 8;
    var s = 256;
    var height = window.screen.height;
    var rows =  Math.round(height/s);
    var num_pizza = rows * cols;

    for (var i = 0; i < num_pizza; i++) {
        var elem = document.createElement('img');
        elem.className = 'mover';
        elem.src = PIZZA_IMAGE_URL;
        elem.style.height = "100px";
        elem.style.width = "73.333px";
        elem.basicLeft = (i % cols) * s;
        elem.style.top = (Math.floor(i / cols) * s) + 'px';
        document.getElementById('movingPizzas1').appendChild(elem);
    }
    updatePositions();
});
