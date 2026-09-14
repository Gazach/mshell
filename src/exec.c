#define _CRT_SECURE_NO_WARNINGS
#include "exec.h"
#include "mode.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

static const char *afterFirstToken(const char *line) {
    const char *p = line;
    while (*p == ' ') p++;

    if (*p == '"') {
        p++;
        const char *end = strchr(p, '"');
        p = end ? end + 1 : p + strlen(p);
    } else {
        while (*p != ' ' && *p != '\0') p++;
    }

    while (*p == ' ') p++;
    return p;
}

static int hasExtension(const char *path, const char *ext) {
    size_t pathLen = strlen(path);
    size_t extLen = strlen(ext);
    if (pathLen < extLen) return 0;
    return _stricmp(path + pathLen - extLen, ext) == 0;
}

int execFindOnPath(const char *name, char *out, size_t outSize) {
    char pathExt[512];
    DWORD n = GetEnvironmentVariableA("PATHEXT", pathExt, sizeof(pathExt));
    if (n == 0 || n >= sizeof(pathExt)) {
        strcpy(pathExt, ".COM;.EXE;.BAT;.CMD");
    }

    for (char *ext = strtok(pathExt, ";"); ext; ext = strtok(NULL, ";")) {
        DWORD len = SearchPathA(NULL, name, ext, (DWORD)outSize, out, NULL);
        if (len > 0 && len < outSize) return 1;
    }

    // `name` might already carry its own extension (or a path) -
    // let SearchPath take it exactly as given as a last try.
    DWORD len = SearchPathA(NULL, name, NULL, (DWORD)outSize, out, NULL);
    return (len > 0 && len < outSize);
}

int execRun(const char *resolvedPath, const char *originalLine) {
    const char *args = afterFirstToken(originalLine);

    char cmdLine[2048];
    if (hasExtension(resolvedPath, ".bat") || hasExtension(resolvedPath, ".cmd")) {

        snprintf(cmdLine, sizeof(cmdLine), "cmd.exe /c \"\"%s\" %s\"", resolvedPath, args);
    } else {
        snprintf(cmdLine, sizeof(cmdLine), "\"%s\" %s", resolvedPath, args);
    }

    fflush(stdout);
    disableRawMode(); // let the child see a normal console, the same way
                       // nvim restores terminal state before running ":!cmd"

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL ok = CreateProcessA(NULL, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

    if (ok) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    enableRawMode(); // resume the shell's own raw-mode input handling

    if (!ok) {
        printf("Failed to run \"%s\" (error %lu).\n", resolvedPath, (unsigned long)GetLastError());
    }

    return ok ? 1 : 0;
}
