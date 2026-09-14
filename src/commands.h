#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>

typedef struct {
    const char *name;
    const char *category;
    const char *description; // one short line, shown in the help menu
} CommandInfo;

extern const CommandInfo COMMAND_TABLE[];
extern const size_t COMMAND_TABLE_COUNT;

#define COMMAND_MAX_CATEGORIES 8

size_t commandsCollectCategories(const char *out[COMMAND_MAX_CATEGORIES]);

const CommandInfo *commandsNthInCategory(const char *category, int n);

const CommandInfo *commandsFind(const char *name);

#endif // COMMANDS_H
