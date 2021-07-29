#ifndef QSHELL_CORE_H
#define QSHELL_CORE_H

#include <stddef.h>

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

/********************************
 *  @author: ZYmelaii
 *  @brief: 标准指令对象
 *  @param: 
 *  @note: 
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
typedef struct _cmd_s
{

} cmd_t;


/********************************
 *  @author: ZYmelaii
 *  @brief: 核心QShell对象
 *  @param: 
 *  @note: 
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
typedef struct _shell_s
{
	int bValid;

	int bLogin;
	char *user_name;
	char *group_name;

	char workdir[MAX_PATH];

	#define BUF0_SIZE 256
	#define BUF1_SIZE 1024
	char buf_0[BUF0_SIZE];
	char *buf_1/*[BUF1_SIZE]*/;
	char *p_read;
	int read_status_code;
	//@value:
	//	( 0) -> `default`/`unknown error`
	//	(-1) -> `EOF`
	//	( 1) -> `normal`

	//! etc.
} shell_t;

//#- shellio.c -

/********************************
 *  @author: ZYmelaii
 *  @brief: 创建QShell对象
 *  @param: 
 *  @note: 全局只能同时存在一个QShell对象
 *  @usage: 
 *  @return:
 *     # -1: 给出的psh为无效指针，对象创建失败
 *     # 0: 对象创建成功
 *******************************/
int qsh_open(shell_t *psh);

/********************************
 *  @author: ZYmelaii
 *  @brief: 销毁QShell对象
 *  @param: 
 *  @note: 
 *  @usage: 对于无效的QShell对象，`qsh_close`将忽略
 *  @return:
 *     # void: /
 *******************************/
void qsh_close(shell_t *psh);

#endif