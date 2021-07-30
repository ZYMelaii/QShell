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
 *  @brief: ����һ������
 *  @param:
 *     # psh: QShell pointer
 *     # cmdline: /
 *  @note: �����������
 *  @usage: 
 *  @return:
 *     # -1: failed
 *     # 0: done
 *******************************/
int qsh_load_cmdline(shell_t *psh, const char *cmdline);

/********************************
 *  @author: ZYmelaii
 *  @brief: �ӱ�׼����������һ������
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