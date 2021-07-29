#ifndef QSHELL_CMDLINE_H
#define QSHELL_CMDLINE_H

#include "core.h"

//#- cmdline.c -

/********************************
 *  @author: ZYmelaii
 *  @brief: 重置上一次命令行字符串的指针
 *  @param:
 *     # psh: QShell pointer
 *  @note: 不推荐手动调用
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qsh_reset_cmdline_ptr(shell_t *psh);

/********************************
 *  @author: ZYmelaii
 *  @brief: 从交互界面（标准输入流）读入一行命令
 *  @param:
 *     # psh: QShell pointer
 *  @note: 命令行字符数不可超过 BUF0_SIZE+BUF1_SIZE
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qsh_readline(shell_t *psh);

/********************************
 *  @author: ZYmelaii
 *  @brief: 获取只读的命令行字符串
 *  @param:
 *     # psh: QShell pointer
 *  @note: 
 *  @usage: 
 *  @return:
 *     # shell_t::p_read: /
 *******************************/
const char *qsh_get_cmdline(shell_t *psh);

#endif