# Web Optimization Project (C/WASM Edition)

This project is a performance-optimized version of a pizzeria website. The core logic for pizza generation and parallax effects has been converted from JavaScript to **C** and compiled to **WebAssembly (WASM)** for improved execution speed and as a demonstration of high-performance web capabilities.

## 🚀 Features
- **C-Powered Logic**: Random pizza name generation and ingredient selection are handled in C.
- **WASM Parallax**: Background animations are calculated using C's `math.h` and executed via WASM.
- **Optimized Assets**: Images are replaced with high-quality, free online versions to reduce local bloat and improve theme consistency.
- **Fast Rendering**: Designed for 60 FPS smooth scrolling.

## 🛠️ Tech Stack
- **Frontend**: HTML5, CSS3, JavaScript
- **Backend Logic**: C (compiled to WASM via Emscripten)
- **Build System**: Makefile
- **Server**: Python (SimpleHTTPServer)

## 📦 Getting Started

### Prerequisites
- [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) (for compiling C to WASM)
- Python 3.x (for the local server)
- GCC (for native C testing)

### Installation & Build
1. Clone the repository.
2. Compile the C code to WASM using the provided Makefile:
   ```bash
   make wasm
   ```
   This will generate `views/js/pizza.js` and `views/js/pizza.wasm`.

3. (Optional) Compile and run native C tests:
   ```bash
   make test
   ```

### Running the Project
Build and start the C-based web server:
```bash
make run-server
```
Open your browser and visit `http://localhost:8000/views/pizza.html`.

## 📈 Optimizations Made
- **Computational Offloading**: Moved random string manipulation and trigonometric calculations from the JS main thread to WASM.
- **Reduced Memory Overhead**: Replaced large JS arrays with static C string arrays.
- **Asset Optimization**: Integrated CDN-based images to replace local heavy assets.
- **Jank-Free Scrolling**: Optimized the `requestAnimationFrame` loop by utilizing C for position calculations.

## 📂 Project Structure
- `src/pizza.c`: The core C logic for the pizza generator.
- `Makefile`: Build instructions for both WASM and native targets.
- `server.py`: A simple Python server to host the project.
- `views/js/main.js`: The JS glue code that interacts with the WASM module.
- `views/pizza.html`: The main pizzeria page.
