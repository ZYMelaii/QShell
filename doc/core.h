#ifndef QSHELL_CORE_H
#define QSHELL_CORE_H

#include <stddef.h>
#include <stdint.h>

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

typedef int32_t return_t;

/********************************
 *  @author: ZYmelaii
 *  @brief: 核心QShell对象
 *  @param:
 *     # void: /
 *  @note: 
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
struct _shell_s
{
//#- validation -
	/********************************
	 *  @object: bValid -> int
	 *  @value:
	 *     # 0: false
	 *     # 1: true
	 *******************************/
	int bValid; //! QShell对象是否有效

//#- user -
	/********************************
	 *  @object: bLogin -> int
	 *  @value:
	 *     # 0: false
	 *     # 1: true
	 *******************************/
	int bLogin; //! 用户是否登录
	char *user_name; //! 用户名
	char *group_name; //! 组名称

//#- path -
	char workdir[MAX_PATH];

//#- cmdline read-only buffer -
	#define BUF0_SIZE 256
	#define BUF1_SIZE 1024
	char buf_0[BUF0_SIZE]; //! 一级缓冲区 (BUF0_SIZE)
	char *buf_1; //! 二级缓冲区 (BUF1_SIZE)

	/********************************
	 *  @object: p_read -> char*
	 *  @value:
	 *     # NULL: unsetted
	 *     # shell_t::buf_0: length(p_read) <= BUF0_SIZE
	 *     # shell_t::buf_1: BUF0_SIZE < length(p_read) <= BUF1_SIZE
	 *     # ...: length(p_read) > BUF1_SIZE
	 *******************************/
	char *p_read; //! 当前存入的命令行 (read-only)

	/********************************
	 *  @object: read_statuc_code -> int
	 *  @value:
	 *     # 0: read nothing
	 *     # -1: read EOF
	 *     # 1: ok
	 *******************************/
	int read_status_code; //! 从标准输入流写入命令行的状态

//#- cmd -
	struct _cmd_s *prev_cmd;

//#- etc -
};
typedef struct _shell_s shell_t;

/********************************
 *  @author: ZYmelaii
 *  @brief: 标准指令对象
 *  @param:
 *     # void: /
 *  @note: 
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
struct _cmd_s
{
	struct _shell_s *root; //! QShell依赖
	const char *cmd; //! 命令 <=> cmd_t::argv[0]
	/********************************
	 *  @object: argv -> char**
	 *  @value:
	 *     # {..., NULL}: /
	 *******************************/
	char **argv; //! 命令参数
	char *args; //! 命令参数源字符串
	/********************************
	 *  @object: succeed -> int
	 *  @value:
	 *     # 0: to be executed
	 *     # -1: failed
	 *     # 1: done
	 *******************************/
	int succeed; //! 是否执行成功
	return_t ret_code; //! 返回值
};
typedef struct _cmd_s cmd_t;

//#- shellio.c -

/********************************
 *  @author: ZYmelaii
 *  @brief: 创建QShell对象
 *  @param:
 *     # psh: QShell pointer
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
 *     # psh: QShell pointer
 *  @note: 对于无效的QShell对象，`qsh_close`将忽略
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qsh_close(shell_t *psh);

//#- mmem.c -

/********************************
 *  @author: ZYmelaii
 *  @brief: 动态分配内存空间
 *  @param:
 *     # size: size of mem-block
 *  @note: 
 *  @usage: 
 *  @return:
 *     # NULL: failed
 *     # ...: done
 *******************************/
void* qsh_malloc(size_t size);

/********************************
 *  @author: ZYmelaii
 *  @brief: 释放内存空间
 *  @param:
 *     # ptr: mem-block pointer
 *  @note: 
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qsh_free(void *ptr);

/********************************
 *  @author: ZYmelaii
 *  @brief: 字符串拷贝
 *  @param:
 *     # s: source string
 *  @note: 不再需要时调用`qsh_free`释放内存空间
 *  @usage: 
 *  @return:
 *     # NULL: failed
 *     # ...: done
 *******************************/
char* qsh_strdup(const char *s);

#endif