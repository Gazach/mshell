#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

// Shell header
#include "mver.h"
#include "mode.h"
#include "tokenize.h"
#include "cmd/command.h"

int  isRunning = 1; // Flag to check shell running.

void runcmd(char *input) {
    // tokenize() overwrites the spaces in `input` with '\0' to carve out
    // argv[] in place, so by the time we might want to show the user what
    // they actually typed, that information is gone from `input` itself.
    // Snapshot it first so "Unknown Command!" can be unambiguous about
    // what was parsed as the command vs. what was parsed as arguments.
    char original[1024];
    strncpy(original, input, sizeof(original) - 1);
    original[sizeof(original) - 1] = '\0';

    char *argv[MAX_ARGS];
    int argc = tokenize(input, argv);

    if (argc == 0) {
        // empty input - do nothing
        return;
    }

    // Basic

    if (strcmp(argv[0], "echo") == 0) {

        cmd_echo(argc, argv);
        
    } else if (strcmp(argv[0], "date") == 0) {
        
        cmd_date(argc, argv);
        
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

         cmd_cp(argc, argv);

    } else if (strcmp(argv[0], "pcd") == 0) {

        cmd_pcd(argc, argv);

    // if command not found.
    } else {
        printf("Unknown Command! : %s\n", argv[0]);
        if (argc > 1) {
            // There's more than one token, meaning tokenize() split on a
            // space somewhere. Show the full original line too, so it's
            // obvious the rest wasn't lost - it was parsed as arguments
            // to a command named argv[0].
            printf("  (parsed as command \"%s\" + %d argument%s - full line was: %s)\n",
                   argv[0], argc - 1, (argc - 1 == 1) ? "" : "s", original);
        }
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

// Redraws the whole prompt + input line from scratch, starting at column 0.
// Unlike redrawTail() (which only patches the delta since the last edit,
// assuming the terminal cursor is already exactly where we think it is),
// this is "self-healing": it never trusts the previous on-screen position,
// so it's what we reach for whenever that trust is broken - e.g. right
// after the console has been resized underneath us.
static void redrawWholeLine(const char *input, size_t len, size_t cursor) {
    printf("\r\x1b[2K"); // \x1b[2K = erase the ENTIRE line, not just cursor-to-end
    printPrompt();
    if (len) printf("%.*s", (int)len, input); // print exactly len bytes - never
                                               // trust a NUL terminator to be in
                                               // the right place
    if (cursor < len) printf("\x1b[%zuD", len - cursor);
    fflush(stdout);
}

void runPrompt(void){
    enableRawMode();

    PrintVer();

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

    char input[1024]; // User Type input
    size_t len = 0;    // bytes currently in the buffer
    size_t cursor = 0; // where in the buffer the terminal cursor sits

    printPrompt();

    INPUT_RECORD ir;
    DWORD read;

    while (isRunning && ReadConsoleInputA(hStdin, &ir, 1, &read)){
        if (ir.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            // Just re-anchor the current line; there's no custom buffer
            // geometry to resync anymore since we write straight to the
            // one real console buffer, which the terminal itself resizes.
            redrawWholeLine(input, len, cursor);
            continue;
        }

        if (ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown){continue;}

        WORD vk = ir.Event.KeyEvent.wVirtualKeyCode;
        char c = ir.Event.KeyEvent.uChar.AsciiChar;

        // Windows can report a held/auto-repeating key as a single event
        // with wRepeatCount set to however many times it "really" fired,
        // instead of one event per repeat. Previously we ignored this and
        // always applied the operation exactly once per event, so fast or
        // held key presses could end up applying fewer times than the key
        // actually registered.
        WORD repeat = ir.Event.KeyEvent.wRepeatCount;
        if (repeat < 1) repeat = 1;

        if (vk == VK_LEFT) {
            if (cursor > 0) {
                size_t n = repeat;
                if (n > cursor) n = cursor;
                cursor -= n;
                redrawWholeLine(input, len, cursor);
            }
            continue;

        } else if (vk == VK_RIGHT) {
            if (cursor < len) {
                size_t n = repeat;
                size_t avail = len - cursor;
                if (n > avail) n = avail;
                cursor += n;
                redrawWholeLine(input, len, cursor);
            }
            continue;

        } else if (vk == VK_HOME) {
            if (cursor > 0) {
                cursor = 0;
                redrawWholeLine(input, len, cursor);
            }
            continue;

        } else if (vk == VK_END) {
            if (cursor < len) {
                cursor = len;
                redrawWholeLine(input, len, cursor);
            }
            continue;

        } else if (vk == VK_DELETE) {
            if (cursor < len) {
                size_t n = repeat;
                size_t avail = len - cursor;
                if (n > avail) n = avail;
                memmove(&input[cursor], &input[cursor + n], len - cursor - n);
                len -= n;
                input[len] = '\0';
                redrawWholeLine(input, len, cursor);
            }
            continue;
        }

        if (c == 0) continue; // other non-character keys (shift, ctrl, alt...)

        if (c == '\r'){  //enter
            input[len] = '\0';
            printf("\r\n"); // \r first: guarantee the new row starts at column 0
                             // even if the cursor had drifted off its expected spot
            runcmd(input);

            if (!isRunning) break;

            len = 0;
            cursor = 0;
            // Erase cursor-to-end-of-SCREEN (not just end-of-line) before
            // printing the new prompt. This is a defensive measure against
            // an apparent Windows Terminal rendering bug where rows being
            // reused as they scroll into history can retain stale glyphs
            // from whatever was drawn there before, even though our program
            // never wrote that content to this row.
            printf("\x1b[J");
            printPrompt();

        } else if ( c == '\b') {  //backspace
            if (cursor > 0){
                size_t n = repeat;
                if (n > cursor) n = cursor;
                memmove(&input[cursor - n], &input[cursor], len - cursor);
                len -= n;
                cursor -= n;
                input[len] = '\0';
                redrawWholeLine(input, len, cursor);
            }
        } else if (len < sizeof(input) - 1){
            size_t n = repeat;
            size_t roomLeft = sizeof(input) - 1 - len;
            if (n > roomLeft) n = roomLeft;
            if (n > 0) {
                memmove(&input[cursor + n], &input[cursor], len - cursor);
                memset(&input[cursor], c, n);
                len += n;
                cursor += n;
                input[len] = '\0'; // insert never terminated the string here before -
                                   // printf("%s", ...) would walk past real content
                                   // into whatever bytes the previous command left behind
                redrawWholeLine(input, len, cursor);
            }
        }
    }
}

int main(int argc, char *argv[]){
    // Title
    printf("\033]0;Moon Shell\007");
    fflush(stdout); // Force the terminal to process the buffer immediately
    
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