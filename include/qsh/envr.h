#ifndef QSHELL_ENVR_H
#define QSHELL_ENVR_H

#include "core.h"

/********************************
 *  @author: ZYmelaii
 *
 *  @sstrmap_t: intro for sstrmap_t
 *
 *  @brief: 字符串对字符串映射哈希表
 *
 *  @NOTES: 由`var.c`管理
 *******************************/
typedef hashmap_t sstrmap_t;

/********************************
 * @author: ZYmelaii
 *
 * @context_t: object which manages QShell environment.
 *
 * @brief: once QShell Ui is setted-up, a root context
 * will be automatically created, after which a secondary
 * context deriving from root will be builded for QShell
 * instance.
 * if QShell runs a single plain statement by `qsh_eval`
 * (from `eval.h`), no extra context will be builded.
 * however, if QShell tries to run a statement containing
 * lower-class segment e.g. `if`, `while`, etc. it'll
 * build a temparary context depriving from current one
 * and destroy it afterward.
 *
 * @NOTES: 1. not every context will deprive from another one.
 * 2. facing symbol conflicts, deprived context has a higher priority.
 *******************************/
typedef struct _context_s
{
	/********************************
	 *  @object: context_t::cid -> int
	 *  @value:
	 *     # -1: invalid context
	 *     # 0: root
	 *     # 1: QShell (user)
	 *     # ...: others
	 *******************************/
	int cid; //! 上下文ID

	/********************************
	 *  @object: symmap -> sstrmap_t*
	 *  @value:
	 *     # context_t::cid == 0: system variables
	 *     # context_t::cid == 1: QShell (builtin|user) variables
	 *     # ...: local variables
	 *******************************/
	sstrmap_t *symmap; //! 符号表

	struct _context_s *parent; //! 父级上下文
	struct _context_s *root; //! 根环境
} context_t;

//#- var.c -
void qsh_var_new(context_t *ctx, const char *key); //! 创建变量
void qsh_var_del(context_t *ctx, const char *key); //! 销毁变量
void qsh_var_get(context_t *ctx, const char *key, const char **pvalue); //! 获取变量值
void qsh_var_set(context_t *ctx, const char *key, const char *value); //! 写入变量

#endif
