#ifndef QSHELL_CMDLINE_H
#define QSHELL_CMDLINE_H

#include "core.h"

//#- cmdline.c -

/********************************
 *	@author: ZYmelaii
 *	@brief: ������һ���������ַ�����ָ��
 *	@param: 
 *	@note: ���Ƽ��ֶ�����
 *	@usage: 
 *	@return:
 *     # void: /
********************************/
void qsh_reset_cmdline_ptr(shell_t *psh);

/********************************
 *	@author: ZYmelaii
 *	@brief: �ӽ������棨��׼������������һ������
 *	@param: 
 *	@note: �������ַ������ɳ���
 *	@usage: 
 *	@return:
 *     # void: /
********************************/
void qsh_readline(shell_t *psh);

/********************************
 *	@author: ZYmelaii
 *	@brief: ��ȡֻ�����������ַ���
 *	@param: 
 *	@note: 
 *	@usage: 
 *	@return:
 *     # void: /
********************************/
const char *qsh_get_cmdline(shell_t *psh);

#endif