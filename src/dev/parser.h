#ifndef QSHELL_PARSER
#define QSHELL_PARSER

#include "core.h"

#ifdef QSHELL_BUILD
#include "syntax.h"
#include "cmdflags.h"
#endif

typedef struct _word_s
{
	int flags;
	char *word;
} word_t;

typedef struct _wordlist_s
{
	word_t *word;
	struct _wordlist_s *next;
} wordlist_t;

enum ecmdtype {

	//! IF语句
	ect_if,
	//! if cond; then cmds; elif cond; then cmds; else cmds; fi
	//! syntax: if cond; then cmds; [elif cond; then cmds;] [else cmds;] fi

	//! CASE 语句
	ect_case,
	//! case var in
	//!     pattern1) command-list;;
	//!     pattern2) command-list;;
	//!     pattern...) command-list;;
	//!     *) command-list;;
	//! esac
	//! syntax: case word in [ [(] pattern [| pattern]...) command-list ;;]... esac

	//! FOR 语句	
	ect_for,
	//! for-arith: for ((init; cond; step)); do command-list; done
	//! for-in: for var in list; do command-list; done

	//! WHILE 语句
	ect_while,
	//! while cond
	//! do
	//!     command-list
	//! done
	//! syntax: while cond; do command-list; done

	//! UNTIL 语句
	ect_until,
	//! until cond
	//! do
	//!     command-list
	//! done
	//! syntax: until cond; do command-list; done

	//! 函数定义
	ect_funcdef,
	//! simple: funcname(args...) { CODESEG... }
	//! declare: function funcname(args...) { CODESEG... }

	//! SELECT 语句
	ect_select,
	//! syntax: select word [in words...]; do commands; done

	//! COPROC 语句
	ect_coproc,
	//! syntax: coproc [NAME] command [redirections]

	//! 简单命令
	ect_sigle,
	//! ...

	//! 条件命令
	ect_cond,
	//! IGNORE

	//! 算术命令
	ect_arith,
	//! IGNORE

	//! 关系组合命令
	ect_conn,
	//! linear: cmd1; cmd2; cmd...
	//! while: cmd1 && cmd2 && cmd...
	//! until: cmd1 || cmd2 || cmd...

	//! 聚合命令
	ect_group,
	//! IGNORE

	//! 子环境执行
	ect_subshell,
	//! 1. execute local script
	//! 2. (cmd1; cmd2; cmd...)
	//! 3. async execution: cmd &
	//! 4. pipeline
	//! 5. redirect
	//! 6. command substitution: `cmd...` or $(cmd...)

};

typedef struct _command_s
{
	int flags;
	enum ecmdtype type;
	union {
		struct _cmd_if_s       *c_if;
		struct _cmd_case_s     *c_case;
		struct _cmd_for_s      *c_for;
		struct _cmd_while_s    *c_while;
		struct _cmd_until_s    *c_until;
		struct _cmd_funcdef_s  *c_funcdef;
		struct _cmd_select_s  *c_select;
		struct _cmd_single_s   *c_single;
		struct _cmd_cond_s     *c_cond;
		struct _cmd_arith_s    *c_arith;
		struct _cmd_conn_s     *c_conn;
		struct _cmd_group_s    *c_group;
		struct _cmd_subshell_s *c_subshell;
	} body;
} command_t;

typedef struct _cmd_if_s
{
	int flags;
  	command_t *test;
  	command_t *true_case;
  	command_t *false_case;
} cmd_if_t;

typedef struct _patternlist_s
{
	wordlist_t *patterns;
	command_t *action;
	struct _patternlist_s *next;
} patternlist_t;

typedef struct _cmd_case_s
{
	int flags;
	int line; //! where CASE statement starts
	word_t *word; //! word to test
	patternlist_t *clauses;
} cmd_case_t;

typedef struct _cmd_forin_s
{
	word_t *varname;
	wordlist_t *list;
} cmd_forin_t;

typedef struct _cmd_forarith_s
{
	wordlist_t *init;
	wordlist_t *test;
	wordlist_t *step;
} cmd_forarith_t;

typedef struct _cmd_for_s
{
	int flags;
	int line; //! where FOR statement starts
	union {
		cmd_forin_t    *forin;
		cmd_forarith_t *forarith;
	} body;
	command_t *action;
} cmd_for_t;

typedef struct _cmd_while_s
{
	int flags;
	int line; //! where WHILE statement starts
	command_t *test;
	command_t *action;
} cmd_while_t;

typedef struct _cmd_until_s
{
	int flags;
	int line; //! where UNTIL statement starts
	command_t *test;
	command_t *action;
} cmd_until_t;

typedef struct _cmd_funcdef_s
{
	int flags;
	int line; //! where function definition starts
	word_t *funcname;
	command_t *command; //! command-list, ended with null
	char *fnsrc; //! file where function was defined, if any
} cmd_funcdef_t;

typedef struct _cmd_select_s
{
	int flags;
	int line; //! where SELECT statement starts
	word_t *varname;
	wordlist_t *optlist;
	command_t *action;
} cmd_select_t;

typedef struct _cmd_coproc_s
{
	int flags;
	char *name;
	command_t *command;
} cmd_coproc_t;

typedef struct _cmd_single_s
{

} cmd_single_t;

typedef struct _cmd_cond_s
{

} cmd_cond_t;

typedef struct _cmd_arith_s
{

} cmd_arith_t;

typedef struct _cmd_conn_s
{
	int flags;
	command_t *first;
	command_t *second;
} cmd_conn_t;

typedef struct _cmd_group_s
{

} cmd_group_t;

typedef struct _cmd_subshell_s
{

} cmd_subshell_t;

//! word.c
word_t* qshp_word_alloc();
word_t* qshp_word_makebare(word_t *pw, const char *s);
word_t* qshp_word_makeflags(word_t *pw, const char *s);
word_t* qshp_word_make(const char *s);
word_t* qshp_word_copy(word_t *pw);
void qshp_word_free(word_t *pw);

wordlist_t* qshp_wordlist_make(word_t *pw, wordlist_t *pwl_next);
wordlist_t* qshp_wordlist_copy(wordlist_t *pwl);
void qshp_wordlist_free(wordlist_t *pwl);

//! scanner.c
//! 流输入器

typedef struct _scanner_s
{
	char *stream;
	char *p;
	const char *eof;
	int line, col;
} scanner_t;

void qshp_scanner_init(scanner_t *ps);
void qshp_scanner_free(scanner_t *ps);
return_t qshp_scanner_read(scanner_t *ps, const char *strbuf);
return_t qshp_scanner_peek(scanner_t *ps, char *pc);
return_t qshp_scanner_next(scanner_t *ps, char *pc);

//! tokenizer.c
//! 分词器 - 第一步划词

typedef struct _tokenizer_s
{
	scanner_t *scanner;
	wordlist_t *tokens;
} tokenizer_t;

void qshp_tokenizer_bind(tokenizer_t *pt, scanner_t *ps);

//! lexer.c
//! 词法分析器 - 第二步划词

typedef struct _lexer_s
{
	tokenizer_t *tokenizer;
} lexer_t;

//! parser.c
//! 语法解析器
typedef struct _parser_s
{

} parser_t;

#endif