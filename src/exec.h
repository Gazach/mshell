#ifndef EXEC_H
#define EXEC_H

#include <stddef.h>

int execFindOnPath(const char *name, char *out, size_t outSize);

int execRun(const char *resolvedPath, const char *originalLine);

#endif // EXEC_H
