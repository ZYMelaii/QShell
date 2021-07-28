#ifndef QSHELL_QSHW_H
#define QSHELL_QSHW_H

#include "core.h"

#ifndef QSHELL_QSHW_H
#define QSHELL_QSHW_H

#include "core.h"

#define QSHW_WHITE    0
#define QSHW_RED      1
#define QSHW_GREEN    2
#define QSHW_BLUE     3
#define QSHW_YELLOW   4
#define QSHW_MAGENTA  5
#define QSHW_CYAN     6

void qshw_print(int color, const char *format, ...);
void qshw_write_prompt(shell_t *psh);

#endif

void qshw_print(int color, const char *format, ...);
void qshw_write_prompt(shell_t *psh);

#endif
