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

    // We deliberately do NOT create a second console screen buffer for
    // scrollback (no CreateConsoleScreenBuffer / SetConsoleActiveScreenBuffer
    // / SetConsoleWindowInfo). Microsoft's own docs mark those APIs "not
    // recommended," and Windows Terminal's ConPTY layer has several open,
    // confirmed bugs specifically around them (screen-buffer-size desync,
    // content leaking into the wrong buffer, buffers not restored on exit).
    // See https://learn.microsoft.com/en-us/windows/console/setconsoleactivescreenbuffer
    // and the "Product-Conpty" label on https://github.com/microsoft/terminal.
    //
    // We write straight to the single real console buffer instead, the way
    // every normal CLI tool does. The terminal's own native scrollback
    // (mouse wheel, scrollbar, search, effectively unlimited history) just
    // works with zero custom code, and this entire class of corruption -
    // where the screen shows content that was never really in our input
    // buffer - becomes structurally impossible, because there's no second
    // buffer left for the host to get out of sync with.
    //
    // ENABLE_MOUSE_INPUT and ENABLE_QUICK_EDIT_MODE are mutually exclusive;
    // we ask for QUICK_EDIT so click-drag select/copy keeps working.
    DWORD raw = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_QUICK_EDIT_MODE;

    SetConsoleMode(hStdin, raw);
}
