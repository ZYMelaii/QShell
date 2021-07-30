#include <assert.h>

#include "eval.h"

#define MASK_ALL      0xffff //! 字符代码区域

//#- 分类码 -
#define MASK_CTRL     0x8000 //! 控制字符
#define MASK_FN       0x4000 //! 其他功能字符
#define MASK_DIGIT    0x2000 //! 数字
#define MASK_ALPHABET 0x1000 //! 字母

//#- 属性码 -
#define MASK_SYMBOL   0x0100 //! 可用于符号命名
#define MASK_ESCAPE   0x0200 //! 特殊字符
#define MASK_QUOTE    0x0400 //! 引号
#define MASK_BRACKET  0x0800 //! 括号

//#- 编号码 -
#define MASK_ID       0x00ff //! 字符组内代码

#define SMASK(code, mask) (((code)&MASK_ALL)&mask)

//#- chrmap.c -
extern int *_chr_code_map;

#define T_DEFAULT 0 //! 常规
#define T_SYMBOL  1 //! 符号
#define T_QUOTE   2 //! 引用
#define T_VAR     3 //! 变量

typedef struct _token_s
{
	const char *t_start;
	int len;
	int T;
	struct _token_s *next;
} token_t;

typedef struct _lexer_s
{
	const char *src;
	const char *cur;
	int T; //! pattern
	int ecode; //! extra code info
	token_t *root;
} lexer_t;

int qshc_get_chr_code(char ch)
{
	return _chr_code_map[(int)ch];
}

void qshc_free_token_list(token_t *root)
{
	token_t *node = root;
	while (node != NULL)
	{
		token_t *tmp = node;
		node = node->next;
		qsh_free(tmp);
	}
}

void qshc_init_lexer(lexer_t *plx, const char *cmdline)
{
	assert(plx != NULL);

	plx->src = qsh_strdup(cmdline);
	plx->cur = plx->src;
	plx->T = T_DEFAULT;
	plx->ecode = 0;
	plx->root = NULL;
}

void qshc_free_lexer(lexer_t *plx)
{
	if (plx == NULL) return;

	if (plx->src != NULL) qsh_free(plx->src);
	qshc_free_token_list(plx->root);
	memset(plx, 0, sizeof(lexer_t));
}