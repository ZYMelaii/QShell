#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "parser.h"

void qshp_scanner_init(scanner_t *ps)
{
	assert(ps != NULL);

	memset(ps, 0, sizeof(scanner_t));
}

void qshp_scanner_free(scanner_t *ps)
{
	if (ps == NULL) return;

	if (ps->stream != 0)
	{
		qsh_free(ps->stream);
	}

	memset(ps, 0, sizeof(scanner_t));
}

/********************************
 *  @author: ZYmelaii
 *  @brief: 
 *  @param:
 *     # void: /
 *  @note: 
 *  @usage: 
 *  @return:
 *     # -2: bad alloc
 *     # -1: bad params
 *     # 0: done
 *******************************/
return_t qshp_scanner_read(scanner_t *ps, const char *strbuf)
{
	if (ps == NULL || strbuf == NULL)
	{
		return -1;
	}

	if (*strbuf == '\0')
	{
		return -1;
	}

	if (ps->stream != NULL)
	{
		qsh_free(ps->stream);
		ps->stream = NULL;
	}

	ps->stream = qsh_strdup(strbuf);
	if (ps->stream == NULL)
	{
		return -2;
	}

	ps->p = ps->stream;
	ps->eof = ps->stream + strlen(ps->stream);
	ps->line = 1;
	ps->col = 0;

	return 0;
}

/********************************
 *  @author: ZYmelaii
 *  @brief: 
 *  @param:
 *     # void: /
 *  @note: 
 *  @usage: 
 *  @return:
 *     # -1: bad params
 *     # 0: done
 *     # 1: reach eof
 *******************************/
return_t qshp_scanner_peek(scanner_t *ps, char *pc)
{
	if (ps == NULL || pc == NULL)
	{
		return -1;
	}

	if (ps->p == ps->eof)
	{
		*pc = '\0';
		return 1;
	}

	*pc = *(ps->p + 1);

	return 0;
}

/********************************
 *  @author: ZYmelaii
 *  @brief: 
 *  @param:
 *     # void: /
 *  @note: 
 *  @usage: 
 *  @return:
 *     # -1: bad params
 *     # 0: done
 *     # 1: reach eof
 *******************************/
return_t qshp_scanner_next(scanner_t *ps, char *pc)
{
	if (ps == NULL || pc == NULL)
	{
		return -1;
	}

	if (ps->p == ps->eof)
	{
		*pc = '\0';
		return 1;
	}

	*pc = *++ps->p;

	if (*pc == '\n')
	{
		++ps->line;
		ps->col = 0;
	} else
	{
		++ps->col;
	}

	return 0;
}