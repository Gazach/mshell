#ifndef COMMAND_H
#define COMMAND_H

#include <windows.h>
#include <stdio.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

#define MAX_ENTRIES 1024

// File and directory.
void cmd_cd(int argc, char *argv[]);
void cmd_ls(int argc, char *argv[]);
void cmd_blank(int argc, char *argv[]);
void cmd_mkdir(int argc, char *argv[]);
void cmd_rm(int argc, char *argv[]);
void cmd_rmdir(int argc, char *argv[]);
void cmd_pcd(int argc, char *argv[]);
void cmd_mv(int argc, char *argv[]);
void cmd_cp(int argc, char *argv[]);

#endif // COMMAND_H
