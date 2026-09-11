#include <string.h>
#include "tokenize.h"

int tokenize(char *input, char *argv[MAX_ARGS]) {
    int argc = 0;
    char *p = input;

    while (*p != '\0' && argc < MAX_ARGS) {
        // skip spaces between tokens
        while (*p == ' ') p++;
        if (*p == '\0') break;

        if (*p == '"') {
            // quoted token: "folder with space" -> one token, quotes stripped
            p++; // skip opening quote
            argv[argc++] = p;

            char *end = strchr(p, '"');
            if (end == NULL) {
                // unterminated quote - just take the rest of the string
                break;
            }

            *end = '\0';
            p = end + 1;
        } else {
            // plain token: ends at the next space
            argv[argc++] = p;

            while (*p != ' ' && *p != '\0') p++;
            if (*p == ' ') {
                *p = '\0';
                p++;
            }
        }
    }

    return argc;
}
