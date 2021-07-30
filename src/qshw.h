#ifndef QSHELL_QSHW_H
#define QSHELL_QSHW_H

#include <stdarg.h>

#include "core.h"

/********************************
 *  @author: ZYmelaii
 *  @brief: 颜色代码
 *  @param:
 *     # void: /
 *  @note: 使用对象为`qshw_print`
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
#define QSHW_WHITE		0
#define QSHW_RED		1
#define QSHW_GREEN		2
#define QSHW_BLUE		3
#define QSHW_YELLOW		4
#define QSHW_MAGENTA	5
#define QSHW_CYAN		6

/********************************
 *  @author: ZYmelaii
 *  @brief: QShell标准输出，允许自定义颜色
 *  @param:
 *     # color: color code, e.g. QSHW_WHITE indicates white
 *     # format: format string or simply literal string
 *     # ...: appended arguments for `format`
 *  @note: 
 *  @usage: qshw_print(QSHW_RED, "[ERROR] msg: %s; errno code: %d", sMsg, errCode)
 *  @return:
 *     # void: /
 *******************************/
void qshw_print(int color, const char *format, ...);

/********************************
 *  @author: ZYmelaii
 *  @brief: 打印提示符
 *  @param:
 *     # psh: QShell pointer
 *  @note: 提示符的内容由QShell决定
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qshw_write_prompt(shell_t *psh);

#endif