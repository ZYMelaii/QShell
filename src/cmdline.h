#ifndef QSHELL_CMDLINE_H
#define QSHELL_CMDLINE_H

#include "core.h"

//#- cmdline.c -
void qsh_reset_cmdline_ptr(shell_t *psh);
void qsh_readline(shell_t *psh);
const char *qsh_get_cmdline(shell_t *psh);

#endif