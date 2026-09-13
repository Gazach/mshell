#include "mode.h"
#include <stdio.h>
#include <string.h>

HANDLE hStdin;
DWORD  origInMode;

void enableVTMode(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD outMode;
    GetConsoleMode(hOut, &outMode);
    SetConsoleMode(hOut, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void disableRawMode(){
    SetConsoleMode(hStdin, origInMode);
}

// Activate raw mode.
void enableRawMode(){
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &origInMode);

    enableVTMode();

    atexit(disableRawMode);

    DWORD raw = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_QUICK_EDIT_MODE;

    SetConsoleMode(hStdin, raw);
}
