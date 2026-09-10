#include "mode.h"
#include <stdio.h>
#include <string.h>

HANDLE hStdin;
DWORD origMode;

void enableVTMode(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD outMode;
    GetConsoleMode(hOut, &outMode);
    SetConsoleMode(hOut, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void enterAltScreen(void) {
    printf("\x1b[?1049h"); // switch to alternate screen buffer
    printf("\x1b[2J");     // clear it
    printf("\x1b[H");      // move cursor to top-left
    fflush(stdout);
}

void exitAltScreen(void) {
    printf("\x1b[?1049l"); // switch back to the normal screen
    fflush(stdout);
}

// Disabling and turn the console to normal console.
void disableRawMode(){    
    exitAltScreen();
    SetConsoleMode(hStdin, origMode);
}

// Activate raw mode.
void enableRawMode(){
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &origMode);
    
    
    enableVTMode();
    enterAltScreen();

    
    atexit(disableRawMode);
    
    DWORD raw = origMode;
    raw &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT); // No echo and line buffer
    
    
    SetConsoleMode(hStdin, raw);
}

