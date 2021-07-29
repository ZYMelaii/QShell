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

//#- parse.c -

/********************************
 *  @author: ZYmelaii
 *  @brief: 解析命令行字符串，判别模式并创建CMD对象
 *  @param:
 *     # psh: QShell pointer
 *     # cmdline: cmdline string to be parsed (read-only)
 *     # qc: QCmd pointer (to be created)
 *  @note: 
 *  @usage: 创建CMD对象当且仅当命令模式为内建函数或外部指令
 *  @return:
 *     # -1: 无效命令
 *     # 0: 语句
 *     # 1: 内建函数
 *     # 2: 外部指令
 *******************************/
int qsh_parse(shell_t *qsh, const char *cmdline, cmd_t *qc);

#endif