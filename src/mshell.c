#include <stdio.h>
#include <string.h>

// Shell header
#include "mode.h"

int  isRunning = 1; // Flag to check shell running.

void runcmd(char *input) {
    while (*input == ' ') input ++;
    
    // Primary Command
    if (strncmp(input, "echo", 4) == 0 && (input[4] == ' ' || input[4] == '\0')){
        char *arg = input + 4;
        while (*arg == ' ') arg++;
        printf("%s\n", arg);
    } else if (strcmp(input, "exit") == 0){
        printf("Exit...\n");
        isRunning = 0;
    } else if (input[0] == '\0'){
        // do nothing
    } else {
        printf("Unknown Command! : %s\n", input);
    }
}

void runPrompt(void){
    enableRawMode();
    
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    
    char input[1024]; // User Type input
    size_t len = 0;
    
    printf("mShell :> ");
    fflush(stdout);
    
    INPUT_RECORD ir;
    DWORD read;

    while (isRunning && ReadConsoleInputA(hStdin, &ir, 1, &read)){
        // ignore key up event
        if (ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown){continue;}
        
        char c = ir.Event.KeyEvent.uChar.AsciiChar;
        if (c == 0) continue;
        
        if (c == '\r'){  //enter
            input[len] = '\0';
            printf("\n");
            runcmd(input);
            
            if (!isRunning) break;
            
            len = 0;
            printf("mShell :> ");
            fflush(stdout);
            
        } else if ( c == '\b') {  //backspace
            if (len > 0){
                len--;
                printf("\b \b");
                fflush(stdout);
            }
        } else if (len < sizeof(input) - 1){
            input[len++] = c;
            putchar(c);
            fflush(stdout);   
        }
    }
}

int main(){
    // running shell
    runPrompt();
    
    return 0;
}