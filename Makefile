CC = gcc
EMCC = emcc
CFLAGS = -Wall -Wextra -O2
EMCCFLAGS = -O3 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=cwrap -s EXPORTED_FUNCTIONS='_get_random_pizza_name,_get_random_pizza_ingredients,_calculate_parallax,_get_pizza_size_ratio,_calculate_dx,_malloc,_free'

SRC = src/pizza.c
SERVER_SRC = src/server.c
OBJ = src/pizza.o
WASM_OUT = views/js/pizza.js
SERVER_OUT = server_native

all: wasm server

wasm:
	$(EMCC) $(EMCCFLAGS) $(SRC) -o $(WASM_OUT)

# Native build disabled: pizza.c requires emscripten.h (Emscripten toolchain only), not available on macOS
native: 
	@echo "Native build skipped: Emscripten headers not available on macOS"
	@exit 0

server:
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER_OUT)

test: |
	@echo "Test skipped: Native build requires Emscripten toolchain for pizza.c (WASM context)"
	@exit 0

run-server: server
	./$(SERVER_OUT)

clean:
	rm -f src/*.o pizza_native $(SERVER_OUT) views/js/pizza.js views/js/pizza.wasm

.PHONY: all wasm native server test run-server clean
