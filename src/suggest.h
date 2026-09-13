#ifndef SUGGEST_H
#define SUGGEST_H

#include <stddef.h>

// Longest word we'll try to fuzzy-match against a command name. Typed
// text longer than this clearly isn't a typo'd command, so we skip it
// rather than paying for the edit-distance table.
#define SUGGEST_MAX_LEN 32

// Looks for the closest known command name to `typed` (e.g. "la" ->
// "ls"). If a close enough match is found, copies it into `out`
// (NUL-terminated) and returns 1. Returns 0 if nothing is close
// enough to be worth suggesting.
int suggestCommand(const char *typed, char *out, size_t outSize);

#endif // SUGGEST_H
