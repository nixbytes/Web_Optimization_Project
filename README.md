# 🍕 Cam's Pizzeria: High-Performance Web Edition

A demonstration of extreme web optimization by offloading core business logic and mathematical computations from JavaScript to **C via WebAssembly (WASM)**.

## 🚀 The Architecture

This project transforms a standard interactive website into a high-performance application by moving the "heavy lifting" to a compiled language.

### 🧠 C $\rightarrow$ WASM Logic Layer
Instead of relying on the JavaScript engine for random generation and animations, we use a compiled C module:
- **Randomization**: Pizza name and ingredient generation is handled in C using static arrays and `rand()`.
- **Layout Math**: Width calculations for the dynamic pizza grid are performed in C to ensure consistency and speed.
- **Parallax Engine**: The background "floating" pizzas use C's `math.h` (`sinf`) to calculate positions, reducing JS main-thread jank.

### 🌐 Custom C Web Server
To avoid the overhead of heavy server frameworks and ensure the correct delivery of WASM binaries, the project includes a **bespoke HTTP server written in C**.
- **MIME Type Handling**: Correctly serves `.wasm` files as `application/wasm` to prevent browser security blocks.
- **Low Overhead**: A lightweight socket-based implementation for local development.

## 🛠️ Tech Stack
- **Frontend**: HTML5, CSS3, JavaScript (Glue Layer)
- **Core**: C99 (Compiled to WASM via Emscripten)
- **Build System**: GNU Make
- **Server**: C (POSIX Sockets)

## 📦 Getting Started

### Prerequisites
- [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html)
- GCC / Clang
- A POSIX-compliant environment (Linux/macOS)

### Installation & Build
1. **Clone the repository** and enter the directory.
2. **Compile everything** (WASM logic and C Server):
   ```bash
   make all
   ```
3. **Run Native Tests**:
   Verify the C logic independently of the browser:
   ```bash
   make test
   ```

### Running the Project
Start the C-based web server:
```bash
make run-server
```
Visit the site at: **`http://localhost:8000/views/pizza.html`**

## 📈 Optimization Highlights
| Feature | Old JS Implementation | New C/WASM Implementation |
| :--- | :--- | :--- |
| **Randomization** | JS Array access + `Math.random()` | Static C Arrays + `rand()` |
| **Parallax** | `Math.sin()` in JS loop | `sinf()` in WASM |
| **Resize Logic** | JS conditional logic | C-based ratio calculation |
| **Server** | Python/Node.js | Native C Sockets |
| **Assets** | Local heavy images | Optimized CDN Assets |

## 📂 Project Structure
- `src/pizza.c`: The "Brain" of the app (Logic & Math).
- `src/server.c`: The "Heart" of the app (Custom Web Server).
- `Makefile`: The build orchestrator.
- `views/js/main.js`: The "Glue" (DOM interaction $\leftrightarrow$ WASM).
- `views/pizza.html`: The "Face" (User Interface).
