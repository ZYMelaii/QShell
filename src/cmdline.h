#ifndef QSHELL_CMDLINE_H
#define QSHELL_CMDLINE_H

#include "core.h"

//#- cmdline.c -

/********************************
 *  @author: ZYmelaii
 *  @brief: ������һ���������ַ�����ָ��
 *  @param:
 *     # psh: QShell pointer
 *  @note: ���Ƽ��ֶ�����
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qsh_reset_cmdline_ptr(shell_t *psh);

/********************************
 *  @author: ZYmelaii
 *  @brief: �ӽ������棨��׼������������һ������
 *  @param:
 *     # psh: QShell pointer
 *  @note: �������ַ������ɳ��� BUF0_SIZE+BUF1_SIZE
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qsh_readline(shell_t *psh);

/********************************
 *  @author: ZYmelaii
 *  @brief: ��ȡֻ�����������ַ���
 *  @param:
 *     # psh: QShell pointer
 *  @note: 
 *  @usage: 
 *  @return:
 *     # shell_t::p_read: /
 *******************************/
const char *qsh_get_cmdline(shell_t *psh);

#endif