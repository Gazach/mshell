#ifndef HISTORY_H
#define HISTORY_H

#include <stddef.h>

#define HISTORY_MAX  200   // how many past commands we remember
#define HISTORY_LINE 1024  // must match the input buffer size in mshell.c

// Adds a finished command line to history. Call this once, right when
// Enter is pressed, BEFORE the line is handed to tokenize()/runcmd() -
// tokenize() splits the buffer in place, so anything after that only
// has the first token left. Blank lines and an exact repeat of the
// previous entry are skipped. Also ends any in-progress browsing, so
// the next Up press starts back at the newest entry.
void historyPush(const char *line);

// Recall the previous (older) entry. 'current'/'currentLen' is the
// line currently on screen - on the FIRST Up press since the last
// Enter this is saved as the "draft" so historyDown() can restore it
// later. Copies the recalled line into out (NUL-terminated) and
// returns its length, or returns -1 if there's nothing older left.
int historyUp(const char *current, size_t currentLen, char *out, size_t outSize);

// Recall the next (newer) entry. Once you move past the newest
// history entry this restores the original draft line instead.
// Returns the copied length, or -1 if already back on the draft.
int historyDown(char *out, size_t outSize);

#endif // HISTORY_H
