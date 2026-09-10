#include "command.h"

void cmd_cd(char *args){
    // trim leading space
    while (*args == ' ') args++;
    
    // strip tailing new line/whitespace
    size_t len = strlen(args);
    while (len > 0 && (args[len - 1] == ' ' || args[len - 1] == '\r')) {
        args[--len] = '\0';
    }
    
    // strip quotes
    if (len >= 2  && args[0] == '"' && args[len-1] == '"'){
        args[len-1] = '\0';
        args++;
    }
    
    if (*args == '\0'){
        char home[MAX_PATH];
        DWORD n = GetEnvironmentVariableA("USERPROFILE", home, sizeof(home));
        if (n == 0 || n > sizeof(home)){
            printf("cd: could not resolve home directory.\n");
            return;
        }
        if(!SetCurrentDirectoryA(home)){
            printf("cd: cannot access (%s).\n", home);
        }
        return;
    }
    if(!SetCurrentDirectoryA(args)){
        printf("cd: cannot find path (%s).\n", args);
    }
}