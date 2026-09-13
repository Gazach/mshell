#define _CRT_SECURE_NO_WARNINGS
#include "suggest.h"
#include "commands.h"
#include <string.h>

static int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

// Levenshtein edit distance, extended with one extra rule (this makes
// it "Optimal String Alignment" distance): swapping two adjacent
// letters counts as a single edit instead of two substitutions. That
// swap is one of the most common real typing mistakes (sl/ls, pdc/pcd),
// so without this rule those exact cases would score too far away to
// suggest anything.
static int editDistance(const char *a, const char *b) {
    size_t la = strlen(a);
    size_t lb = strlen(b);
    if (la > SUGGEST_MAX_LEN) la = SUGGEST_MAX_LEN; // callers already
    if (lb > SUGGEST_MAX_LEN) lb = SUGGEST_MAX_LEN; // keep both this short

    int dp[SUGGEST_MAX_LEN + 1][SUGGEST_MAX_LEN + 1];

    for (size_t i = 0; i <= la; i++) dp[i][0] = (int)i;
    for (size_t j = 0; j <= lb; j++) dp[0][j] = (int)j;

    for (size_t i = 1; i <= la; i++) {
        for (size_t j = 1; j <= lb; j++) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            int best = min3(dp[i - 1][j] + 1,         // delete a char from a
                             dp[i][j - 1] + 1,         // insert a char into a
                             dp[i - 1][j - 1] + cost); // substitute (or match)

            if (i > 1 && j > 1 &&
                a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
                int swap = dp[i - 2][j - 2] + 1; // adjacent transposition
                if (swap < best) best = swap;
            }

            dp[i][j] = best;
        }
    }
    return dp[la][lb];
}

int suggestCommand(const char *typed, char *out, size_t outSize) {
    size_t typedLen = strlen(typed);
    if (typedLen > SUGGEST_MAX_LEN) return 0; // not going to look like any command name

    const char *best = NULL;
    int bestDist = -1;

    for (size_t i = 0; i < COMMAND_TABLE_COUNT; i++) {
        int d = editDistance(typed, COMMAND_TABLE[i].name);
        if (bestDist < 0 || d < bestDist) {
            bestDist = d;
            best = COMMAND_TABLE[i].name;
        }
    }

    // Scale the "close enough" threshold with how long the typed word
    // is, so a single missing letter still counts on longer commands
    // (e.g. "mkdr" -> "mkdir") without matching unrelated short words.
    int threshold = (typedLen <= 3) ? 1 : (typedLen <= 6 ? 2 : 3);

    if (best && bestDist > 0 && bestDist <= threshold) {
        strncpy(out, best, outSize - 1);
        out[outSize - 1] = '\0';
        return 1;
    }
    return 0;
}
