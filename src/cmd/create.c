// Code that anything to create file/dir

#include "command.h"

void cmd_blank(char *args){
    // trim leading space
    while (*args == ' ') args++;
    
    if (*args == '\0') {
        printf("blank: usage [blank <filename>].\n");
        return;
    }
    
    // strip tailing new line/whitespace
    size_t len = strlen(args);
    while (len > 0 && (args[len - 1] == ' ' || args[len - 1] == '\r')) {
        args[--len] = '\0';
    }
    
    HANDLE hFile = CreateFileA(
        args,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hFile == INVALID_HANDLE_VALUE){
        DWORD err = GetLastError();
        if (err == ERROR_FILE_EXISTS){
            printf("blank: '%s' already exist!.\n", args);
        } else {
            printf("blank: '%s' cannot be create!.\n", args);
        }
        return;
    }
    
    CloseHandle(hFile);
    
}

// make directory
void cmd_mkdir(char *args){
    // trim leading space
    while (*args == ' ') args++;
    
    if (*args == '\0') {
        printf("mkdir: usage [mkdir <foldername>].\n");
        return;
    }
    
    // strip tailing new line/whitespace
    size_t len = strlen(args);
    while (len > 0 && (args[len - 1] == ' ' || args[len - 1] == '\r')) {
        args[--len] = '\0';
    }
    
    if (!CreateDirectoryA(args, NULL)){
        DWORD err = GetLastError();
        if (err == ERROR_ALREADY_EXISTS){
            printf("mkdir: '%s' already exist!.\n", args);
        } else if (err == ERROR_PATH_NOT_FOUND){
            printf("mkdir: cannot create '%s' : parent directory doesnt exist.\n", args);
        } else {
            printf("mkdir: '%s' cannot be create!.\n", args);
        }
        return;
    }
    
}