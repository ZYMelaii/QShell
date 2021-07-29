#ifndef QSHELL_QVAL_H
#define QSHELL_QVAL_H

#include "core.h"

//#- builtin.c -
#define DEF_BUILTIN(func) void qsh_builtin_##func(shell_t*, cmd_t*)
DEF_BUILTIN(help);
DEF_BUILTIN(exit);
DEF_BUILTIN(echo);
DEF_BUILTIN(clear);
DEF_BUILTIN(cd);
DEF_BUILTIN(pwd);
#undef DEF_BUILTIN

#endif