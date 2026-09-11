#include <stdio.h>
#include <string.h>

// Shell header
#include "mver.h"
#include "mode.h"
#include "tokenize.h"
#include "cmd/command.h"

int  isRunning = 1; // Flag to check shell running.

void runcmd(char *input) {
    char *argv[MAX_ARGS];
    int argc = tokenize(input, argv);

    if (argc == 0) {
        // empty input - do nothing
        return;
    }

    //Primary Commands

    if (strcmp(argv[0], "echo") == 0) {

        for (int i = 1; i < argc; i++) {
            printf("%s%s", argv[i], (i < argc - 1) ? " " : "");
        }
        printf("\n");

    } else if (strcmp(argv[0], "exit") == 0) {

        printf("Exit...\n");
        isRunning = 0;

    //File/dir Commands

    } else if (strcmp(argv[0], "cd") == 0) {

        cmd_cd(argc, argv);

    } else if (strcmp(argv[0], "ls") == 0) {

        cmd_ls(argc, argv);

    } else if (strcmp(argv[0], "blank") == 0) {

        cmd_blank(argc, argv);

    } else if (strcmp(argv[0], "mkdir") == 0) {

        cmd_mkdir(argc, argv);

    } else if (strcmp(argv[0], "rmdir") == 0) {

        cmd_rmdir(argc, argv);

    } else if (strcmp(argv[0], "rm") == 0) {

        cmd_rm(argc, argv);

    } else if (strcmp(argv[0], "mv") == 0) {

        cmd_mv(argc, argv);

    } else if (strcmp(argv[0], "cp") == 0) {

        printf("NOT IMPLEMENTED YET\n");

    } else if (strcmp(argv[0], "pcd") == 0) {

        cmd_pcd(argc, argv);

    //Other
    } else {

        printf("Unknown Command! : %s\n", argv[0]);
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
