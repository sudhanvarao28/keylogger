# keylogger Usage

### To run Keylog server:
- .\bin\server_keylogger.exe \<IP> \<Port>
- for linux : ./bin/linux_executables/server_keylogger_linux


### To run keylogger:
- First open main.c and change the IP address and Port number to your preference and compile the code as follows
    
    - gcc -o ../bin/keylogger.exe .\src\main.c .\src\keylogger.c .\src\send_keylog.c  -lws2_32
    - Then go to your bin folder and run keylogger.exe as follows:
        .\keylogger.exe

- For linux: First open main.c and change the IP address and Port number to your preference and compile the code as follows
    
    - gcc -o ./bin/linux_executables/keylogger_linux ./src/for_linux/main.c  ./src/for_linux/keylogger.c  ./src/for_linux/send_keylog.c -lpthread
    - Then go to your bin folder and run keylogger as follows:
        ./linux_executables/keylogger

