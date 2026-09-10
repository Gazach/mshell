#include "command.h"

void cmd_pcd(char *args){
    (void)args; // since there is no argument. then sending the args to void realm
    
    char cwd[MAX_PATH];
    if (GetCurrentDirectoryA(sizeof(cwd), cwd) == 0){
        printf("pcd : cannot get the current directory");
        return;
    }
    
    printf("%s\n", cwd);
}