#include "command.h"

void cmd_pcd(int argc, char *argv[]) {
    (void)argc; // pcd takes no arguments
    (void)argv;

    char cwd[MAX_PATH];
    if (GetCurrentDirectoryA(sizeof(cwd), cwd) == 0) {
        printf("pcd : cannot get the current directory");
        return;
    }

    printf("%s\n", cwd);
}
