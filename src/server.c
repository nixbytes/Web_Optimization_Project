#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

/*
 * BEGINNERS GUIDE TO C NETWORKING:
 * - Socket: Think of a socket as a "plug" that allows your program to talk to a network.
 * - Bind: This tells the OS "I want to listen for messages on this specific port (e.g., 8000)".
 * - Listen: This puts the server in "waiting mode", ready for clients to connect.
 * - Accept: This is when a client (like a browser) actually knocks on the door and the server lets them in.
 * - MIME Type: A label that tells the browser "this file is a WASM file" or "this is an HTML file".
 */

#define PORT 8000
#define BUFFER_SIZE 1024

// Function to determine the content type based on file extension
const char* get_mime_type(const char* path) {
    if (strstr(path, ".html")) return "text/html";
    if (strstr(path, ".css")) return "text/css";
    if (strstr(path, ".js")) return "application/javascript";
    if (strstr(path, ".wasm")) return "application/wasm";
    if (strstr(path, ".png")) return "image/png";
    if (strstr(path, ".jpg")) return "image/jpeg";
    return "application/octet-stream";
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read <= 0) {
        close(client_socket);
        return;
    }
    buffer[bytes_read] = '\0';

    // Very basic parsing of the HTTP GET request
    // Example: "GET /views/pizza.html HTTP/1.1"
    char method[10], path[256], protocol[20];
    sscanf(buffer, "%s %s %s", method, path, protocol);

    printf("Request: %s %s\n", method, path);

    // Default to index.html if root is requested
    char file_path[512];
    if (strcmp(path, "/") == 0) {
        strcpy(file_path, "index.html");
    } else {
        // Remove leading slash to make it a relative path
        sprintf(file_path, "%s", path + 1);
    }

    // Open the file
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        char *error_msg = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(client_socket, error_msg, strlen(error_msg), 0);
        printf("File not found: %s\n", file_path);
    } else {
        // Get file size
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Send HTTP Headers
        char header[256];
        sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n", 
                get_mime_type(file_path), file_size);
        send(client_socket, header, strlen(header), 0);

        // Send file content in chunks
        char file_buffer[BUFFER_SIZE];
        int bytes_read_file;
        while ((bytes_read_file = fread(file_buffer, 1, BUFFER_SIZE, file)) > 0) {
            send(client_socket, file_buffer, bytes_read_file, 0);
        }
        fclose(file);
    }

    close(client_socket);
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // 1. Create the socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Allow port reuse (prevents "Address already in use" error)
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Define the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces
    server_addr.sin_port = htons(PORT);       // Convert port number to network byte order

    // 3. Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 4. Listen for incoming connections
    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("C Web Server running at http://localhost:%d\n", PORT);
    printf("Press Ctrl+C to stop.\n");

    while (1) {
        // 5. Accept a new connection
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        handle_client(client_socket);
    }

    close(server_socket);
    return 0;
}
