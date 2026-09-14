#ifndef HISTORY_H
#define HISTORY_H

#include <stddef.h>

#define HISTORY_MAX  200   // how many past commands we remember
#define HISTORY_LINE 1024  // must match the input buffer size in mshell.c


void historyPush(const char *line);

int historyUp(const char *current, size_t currentLen, char *out, size_t outSize);

int historyDown(char *out, size_t outSize);

#endif // HISTORY_H
