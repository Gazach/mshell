#ifndef COMMAND_H
#define COMMAND_H

#include <windows.h>
#include <stdio.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

#define MAX_ENTRIES 1024

// File and directory.
void cmd_cd(char *args);
void cmd_ls(char *args);

#endif // COMMAND_H