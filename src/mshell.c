#include <stdio.h>
#include <string.h>

// Shell header
#include "mver.h"
#include "mode.h"
#include "cmd/command.h"

int  isRunning = 1; // Flag to check shell running.

void runcmd(char *input) {
    while (*input == ' ') input ++;
    
    //Primary Commands
    
    if (strncmp(input, "echo", 4) == 0 &&
        (input[4] == ' ' || input[4] == '\0')) {
    
        char *arg = input + 4;
    
        while (*arg == ' ')
            arg++;
    
        printf("%s\n", arg);
    
    } else if (strcmp(input, "exit") == 0) {
    
        printf("Exit...\n");
        isRunning = 0;
    
    //File/dir Commands
    
    } else if (strncmp(input, "cd", 2) == 0 &&
               (input[2] == ' ' || input[2] == '\0')) {
    
        cmd_cd(input + 2);
    
    } else if (strncmp(input, "ls", 2) == 0 &&
               (input[2] == ' ' || input[2] == '\0')) {
    
        cmd_ls(input + 2);
    
    } else if (strncmp(input, "blank", 5) == 0 &&
               (input[5] == ' ' || input[5] == '\0')) {
    
        cmd_blank(input + 5);
    
    } else if (strncmp(input, "mkdir", 5) == 0 &&
               (input[5] == ' ' || input[5] == '\0')) {
    
        cmd_mkdir(input + 5);
    
    } else if (strncmp(input, "rmdir", 5) == 0 &&
               (input[5] == ' ' || input[5] == '\0')) {
    
        printf("NOT IMPLEMENTED YET\n");
    
    } else if (strncmp(input, "rm", 2) == 0 &&
               (input[2] == ' ' || input[2] == '\0')) {
    
        printf("NOT IMPLEMENTED YET\n");
    
    } else if (strncmp(input, "mv", 2) == 0 &&
               (input[2] == ' ' || input[2] == '\0')) {
    
        printf("NOT IMPLEMENTED YET\n");
    
    } else if (strncmp(input, "cp", 2) == 0 &&
               (input[2] == ' ' || input[2] == '\0')) {
    
        printf("NOT IMPLEMENTED YET\n");
    
    
    //Other
    
    } else if (input[0] == '\0') {
    
        // Do nothing
    
    } else {
    
        printf("Unknown Command! : %s\n", input);
    }

}

void printPrompt(void) {
    char cwd[MAX_PATH];
    GetCurrentDirectoryA(sizeof(cwd), cwd);

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    //lime
    SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("$MShell ");

    //cyan
    SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("%s", cwd);

    // > -> white
    SetConsoleTextAttribute(console, FOREGROUND_RED |
                                     FOREGROUND_GREEN |
                                     FOREGROUND_BLUE);
    printf("> ");

    fflush(stdout);
}

void runPrompt(void){
    enableRawMode();
    
    PrintVer();
    
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    
    char input[1024]; // User Type input
    size_t len = 0;
    
    printPrompt();
    
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
            printPrompt();
            
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

int main(int argc, char *argv[]){
    if (argc < 2){
        SetCurrentDirectoryA("C:\\");
    } else if (strcmp(argv[1], ".") == 0) {// pass nothing
    } else {
        if (!SetCurrentDirectoryA(argv[1])){
            fprintf(stderr, "Moon Shell cannot find path (%s)", argv[1]);
            return 1;
        }
    }
    
    // running shell
    runPrompt();
    
    return 0;
}