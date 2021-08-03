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
 *
 *  @object_t: intro for object_t
 *
 *  @brief: 通用对象结构
 *
 *  @NOTES: 1. 由相关接口管理具体内容
 *     2. 要求`object_t::data`由`qsh_malloc`分配内存空间并由`qsh_free`释放
 *******************************/
typedef struct _object_s { void *data; } object_t;

/********************************
 *  @author: ZYmelaii
 *
 *  @listnode_t: intro for listnode_t
 *
 *  @brief: 链表节点
 *
 *  @NOTES: 
 *******************************/
typedef struct _listnode_s
{
	object_t obj;
	struct _listnode_s *next;
} listnode_t;

/********************************
 *  @author: ZYmelaii
 *
 *  @pair_t: intro for pair_t
 *
 *  @brief: hashmap_t元素
 *
 *  @NOTES: 
 *******************************/
typedef struct _pair_s
{
	void *key;
	void *value;
} pair_t;

/********************************
 *  @author: ZYmelaii
 *
 *  @hashmap_t: intro for hashmap_t
 *
 *  @brief: 哈希表
 *
 *  @NOTES: 1. 由`hashmap.c`管理
 *     2. 内部使用`listnode_t`结构
 *******************************/
typedef object_t hashmap_t;

/********************************
 *  @author: ZYmelaii
 *
 *  @shell_t: intro for shell_t
 *
 *  @brief: 核心QShell对象
 *
 *  @NOTES: 
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
 *
 *  @cmd_t: intro for cmd_t
 *
 *  @brief: 标准指令对象
 *
 *  @NOTES: 
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
	int argc; //! 命令参数数量
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

//#- hashmap.c -

#define QSH_HASH_MAX 1024 //! 哈希表大小上限
typedef uint32_t (*fn_hash_t)(void *key); //! 通用哈希函数类型
typedef int (*fn_cmp_t)(void *x, void *y); //! 通用匹配函数类型 (x == y -> return 0)
typedef void* (*fn_dup_t)(const void *src); //! 通用副本生成函数类型

void qsh_hashmap_init(hashmap_t *phm, size_t size); //! 创建哈希表
void qsh_hashmap_free(hashmap_t *phm); //! 销毁哈希表

/********************************
 *  @author: ZYmelaii
 *  @brief: hashmap_t添加新键
 *  @param:
 *     # phm: hashmap_t pointer
 *     # key: key value (not necessarily void*)
 *     # hash: hash function
 *     # cmp: compare function
 *     # dup: duplicate function
 *  @note: 
 *  @usage: 
 *  @return:
 *     # 0: done
 *     # 1: key already exists
 *     # -1: failed to allocate memory
 *     # -2: unreliable `fn_dup_t dup` function
 *     # -3: unknown error
 *******************************/
int qsh_hashmap_add(
	hashmap_t *phm, void *key,
	fn_hash_t hash, fn_cmp_t cmp, fn_dup_t dup);

/********************************
 *  @author: ZYmelaii
 *  @brief: hashmap_t删除键
 *  @param:
 *     # phm: hashmap_t pointer
 *     # key: key value (not necessarily void*)
 *     # hash: hash function
 *     # cmp: compare function
 *  @note: 
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qsh_hashmap_del(
	hashmap_t *phm, void *key,
	fn_hash_t hash, fn_cmp_t cmp);

/********************************
 *  @author: ZYmelaii
 *  @brief: hashmap_t获取键值
 *  @param:
 *     # phm: hashmap_t pointer
 *     # key: key value (not necessarily void*)
 *     # hash: hash function
 *     # cmp: compare function
 *  @note: 
 *  @usage: 
 *  @return:
 *     # NULL: the key doesn't exist
 *     # 0xffffffff: undefined key-value
 *     # ...: done
 *******************************/
void* qsh_hashmap_getval(
	hashmap_t *phm, void *key,
	fn_hash_t hash, fn_cmp_t cmp);

/********************************
 *  @author: ZYmelaii
 *  @brief: hashmap_t写入键值
 *  @param:
 *     # phm: hashmap_t pointer
 *     # key: key value (not necessarily void*)
 *     # hash: hash function
 *     # cmp: compare function
 *  @note: 写入键值的具体操作在返回的指针中完成，
 *     结束写入后调用`qsh_hashmap_done`完成操作。
 *  @usage: 
 *  @return:
 *     # NULL: no such key
 *     # ...: done
 *******************************/
object_t* qsh_hashmap_write(
	hashmap_t *phm, void *key,
	fn_hash_t hash, fn_cmp_t cmp);

/********************************
 *  @author: ZYmelaii
 *  @brief: hashmap_t结束键值写入
 *  @param:
 *     # phm: hashmap_t pointer
 *  @note: `与qsh_hashmap_write`成对出现
 *  @usage: 
 *  @return:
 *     # void: /
 *******************************/
void qsh_hashmap_done(hashmap_t *phm);

/********************************
 *  @author: ZYmelaii
 *  @brief: 字符串哈希函数
 *  @param:
 *     # key: 给定字符串
 *  @note: 使用DJBHash算法
 *  @usage: 
 *  @return:
 *     # ...: 字符串hash值
 *******************************/
uint32_t qsh_hash_str(void *key);

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
 *  @brief: 获取分配的内存空间大小
 *  @param:
 *     # ptr: mem-block pointer
 *  @note: 
 *  @usage: 
 *  @return:
 *     # -1: 无效的内存块
 *     # ...: 内存块大小
 *******************************/
size_t qsh_msize(void *ptr);

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