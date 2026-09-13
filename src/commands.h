#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>

// One row per built-in command. This is the single place that lists
// every command's name/category/description - suggest.c measures
// typos against it, and cmd_help() builds its menu from it. Add a new
// command here too (alongside its runcmd() dispatch in mshell.c) so
// both stay in sync automatically.
typedef struct {
    const char *name;
    const char *category;
    const char *description; // one short line, shown in the help menu
} CommandInfo;

extern const CommandInfo COMMAND_TABLE[];
extern const size_t COMMAND_TABLE_COUNT;

#define COMMAND_MAX_CATEGORIES 8

// Collects each distinct category name (in first-seen order) into
// `out`, up to COMMAND_MAX_CATEGORIES entries. Returns how many it found.
size_t commandsCollectCategories(const char *out[COMMAND_MAX_CATEGORIES]);

// Finds the command shown as choice number `n` (1-based, matching what
// the help menu lists on screen) within `category`. Returns NULL if
// `n` is out of range for that category.
const CommandInfo *commandsNthInCategory(const char *category, int n);

// Finds a command by its exact name (e.g. for "help ls"). Returns
// NULL if there's no such command.
const CommandInfo *commandsFind(const char *name);

#endif // COMMANDS_H
