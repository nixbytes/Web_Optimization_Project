CC = gcc
EMCC = emcc
CFLAGS = -Wall -Wextra -O2
EMCCFLAGS = -O3 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=['cwrap'] -s EXPORTED_FUNCTIONS=['_get_random_pizza_name', '_get_random_pizza_ingredients', '_calculate_parallax', '_get_pizza_size_ratio', '_calculate_dx', '_malloc', '_free']

SRC = src/pizza.c
SERVER_SRC = src/server.c
OBJ = src/pizza.o
WASM_OUT = views/js/pizza.js
SERVER_OUT = server_native

all: wasm native server

wasm:
	$(EMCC) $(EMCCFLAGS) $(SRC) -o $(WASM_OUT)

native:
	$(CC) $(CFLAGS) $(SRC) -o pizza_native -lm

server:
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER_OUT)

test: native
	./pizza_native

run-server: server
	./$(SERVER_OUT)

clean:
	rm -f src/*.o pizza_native $(SERVER_OUT) $(WASM_OUT) views/js/pizza.wasm

.PHONY: all wasm native server test run-server clean
