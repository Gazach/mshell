// Code that anything to remove file/dir

#include "command.h"

void cmd_rm(char *args){
    // trim leading space
    while (*args == ' ') args++;
    
    if (*args == '\0') {
        printf("rm: usage [rm <filename>].\n");
        return;
    }
    
    // strip tailing new line/whitespace
    size_t len = strlen(args);
    while (len > 0 && (args[len - 1] == ' ' || args[len - 1] == '\r')) {
        args[--len] = '\0';
    }
    
    if (!DeleteFileA(args)){
        DWORD err = GetLastError();
        
        if (err == ERROR_FILE_NOT_FOUND){
            printf("rm: '%s' not found!.\n", args);
        } else if (err == ERROR_ACCESS_DENIED){
            printf("rm: '%s' is read-only or in use.\n", args);
        } else {
            printf("rm: '%s' cannot be removed!.\n", args);
        }
        return;
    }
    
}

// make directory
void cmd_rmdir(char *args){
    // trim leading space
    while (*args == ' ') args++;
    
    if (*args == '\0') {
        printf("rmdir: usage [rmdir <foldername>].\n");
        return;
    }
    
    // strip tailing new line/whitespace
    size_t len = strlen(args);
    while (len > 0 && (args[len - 1] == ' ' || args[len - 1] == '\r')) {
        args[--len] = '\0';
    }
    
    if (!RemoveDirectoryA(args)){
        DWORD err = GetLastError();
        if (err == ERROR_DIR_NOT_EMPTY){
            printf("rmdir: '%s' is not empty!.\n", args);
        } else if (err == ERROR_PATH_NOT_FOUND || err == ERROR_FILE_NOT_FOUND){
            printf("rmdir: cannot find '%s'.\n", args);
        } else {
            printf("rmdir: '%s' cannot be removed!.\n", args);
        }
        return;
    }
    
}