#include "mode.h"
#include <stdio.h>
#include <string.h>

HANDLE hStdin;
DWORD  origInMode;
static int atexitRegistered = 0;

void enableVTMode(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD outMode;
    GetConsoleMode(hOut, &outMode);
    SetConsoleMode(hOut, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void disableRawMode(){
    SetConsoleMode(hStdin, origInMode);
}

// Activate raw mode. Safe to call more than once per session (e.g.
// after disableRawMode() was used to hand the console to a child
// process) - it just re-captures whatever mode is current and reapplies
// raw mode on top of it, without registering a duplicate atexit handler.
void enableRawMode(){
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &origInMode);

    enableVTMode();

    if (!atexitRegistered) {
        atexit(disableRawMode);
        atexitRegistered = 1;
    }

    DWORD raw = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_QUICK_EDIT_MODE;

    SetConsoleMode(hStdin, raw);
}
