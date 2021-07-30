#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdline.h"

void qsh_reset_cmdline_ptr(shell_t *psh)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);

	psh->read_status_code = 0;
	if (psh->p_read == NULL) return;

	if (psh->p_read != psh->buf_0 && psh->p_read != psh->buf_1)
	{
		qsh_free(psh->p_read);
	}

	psh->p_read = NULL;
}

int qsh_load_cmdline(shell_t *psh, const char *cmdline)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);

	if (cmdline == NULL) return -1;

	qsh_reset_cmdline_ptr(psh);

	int len = strlen(cmdline);
	if (len <= BUF0_SIZE)
	{
		strcpy(psh->buf_0, cmdline);
		psh->p_read = psh->buf_0;
	} else if (len <= BUF1_SIZE)
	{
		strcpy(psh->buf_1, cmdline);
		psh->p_read = psh->buf_1;
	} else
	{
		char *s_cmdline = qsh_strdup(cmdline);
		if (s_cmdline == NULL) return -1;
		psh->p_read = s_cmdline;
	}

	return 0;
}

void qsh_readline(shell_t *psh)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);

	qsh_reset_cmdline_ptr(psh);

	char format[32];

	sprintf(format, "%%%d[^\n]", BUF0_SIZE); //! 限制最大读取数，放置越界
	int bRet = scanf(format, psh->buf_0);

	if (bRet <= 0)
	{
		if (bRet == -1)
		{
			psh->read_status_code = -1;
		}
		fflush(stdin);
		return;
	}

	int nCount = strlen(psh->buf_0);
	if (nCount == BUF0_SIZE)
	{	// overflow - 1
		sprintf(format, "%%%d[^\n]", BUF1_SIZE);
		bRet = scanf(format, psh->buf_1);

		fflush(stdin);
		nCount = strlen(psh->buf_1);
		if (nCount == BUF1_SIZE)
		{	// overflow - 2, report an ERROR and exit
			printf("QShell: cmdline is too long.\n");
			return;
		} else if (nCount + BUF0_SIZE > BUF1_SIZE)
		{	// merge buffer
			psh->p_read = (char*)qsh_malloc(BUF0_SIZE + nCount + 1);
			strcat(psh->p_read, psh->buf_0);
			strcat(psh->p_read, psh->buf_1);
		} else if (bRet == 1) {
			char *p = psh->buf_1 + nCount + 1, *q = p + BUF0_SIZE;
			do { *--q = *--p; } while (p != psh->buf_1);
			memcpy(psh->buf_1, psh->buf_0, BUF0_SIZE);
			psh->p_read = psh->buf_1;
		}
	}

	if (psh->p_read == NULL)
	{
		fflush(stdin);
		psh->p_read = psh->buf_0;
	}

	psh->read_status_code = 1;
}

const char *qsh_get_cmdline(shell_t *psh)
{
	return psh->p_read;
}