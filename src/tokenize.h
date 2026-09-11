#ifndef TOKENIZE_H
#define TOKENIZE_H

#define MAX_ARGS 64

// Splits input into argv-style tokens, respecting "quoted strings"
// so paths with spaces (e.g. "folder with space") stay as one token.
//
// NOTE: this modifies the input buffer in place (inserts '\0' at token
// boundaries) - argv[] entries point back into it, nothing is copied.
//
// Returns argc: the number of tokens found (0 if input was empty/blank).
int tokenize(char *input, char *argv[MAX_ARGS]);

#endif // TOKENIZE_H
