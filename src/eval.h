#ifndef QSHELL_QVAL
#define QSHELL_QVAL

#include "core.h"

//#- builtin.c -

#define DEF_BUILTIN(func) void qsh_builtin_##func(cmd_t*)
DEF_BUILTIN(help);
DEF_BUILTIN(exit);
DEF_BUILTIN(echo);
DEF_BUILTIN(clear);
DEF_BUILTIN(cd);
DEF_BUILTIN(pwd);
#undef DEF_BUILTIN

#endif