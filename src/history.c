#define _CRT_SECURE_NO_WARNINGS
#include "history.h"
#include <string.h>

static char entries[HISTORY_MAX][HISTORY_LINE];
static int  count = 0;
static int  head  = 0;

// Browsing state (Up/Down navigation).
static int  browsing = 0; // 0 = editing the live line, 1 = looking at history
static int  dist     = 0; // 0 = newest entry, higher = further into the past
static char draft[HISTORY_LINE];

// logical index 0 = newest, 1 = one before that, etc.
static char *entryFromNewest(int i) {
    int physical = (head + (count - 1 - i)) % HISTORY_MAX;
    return entries[physical];
}

static void copyOut(const char *src, char *out, size_t outSize) {
    strncpy(out, src, outSize - 1);
    out[outSize - 1] = '\0';
}

void historyPush(const char *line) {
    browsing = 0; // running a command always drops you back to a fresh live line

    if (line[0] == '\0') return; // don't remember blank Enters
    if (count > 0 && strcmp(entryFromNewest(0), line) == 0) return; // skip immediate repeats

    int writeAt;
    if (count < HISTORY_MAX) {
        writeAt = (head + count) % HISTORY_MAX;
        count++;
    } else {
        writeAt = head;
        head = (head + 1) % HISTORY_MAX;
    }

    strncpy(entries[writeAt], line, HISTORY_LINE - 1);
    entries[writeAt][HISTORY_LINE - 1] = '\0';
}

int historyUp(const char *current, size_t currentLen, char *out, size_t outSize) {
    if (count == 0) return -1;

    if (!browsing) {
        size_t n = currentLen < sizeof(draft) - 1 ? currentLen : sizeof(draft) - 1;
        memcpy(draft, current, n);
        draft[n] = '\0';
        browsing = 1;
        dist = 0;
    } else {
        if (dist + 1 >= count) return -1; // already at the oldest entry
        dist++;
    }

    copyOut(entryFromNewest(dist), out, outSize);
    return (int)strlen(out);
}

int historyDown(char *out, size_t outSize) {
    if (!browsing) return -1; // already on the live line, nothing below it

    if (dist == 0) {
        browsing = 0;
        copyOut(draft, out, outSize);
        return (int)strlen(out);
    }

    dist--;
    copyOut(entryFromNewest(dist), out, outSize);
    return (int)strlen(out);
}
