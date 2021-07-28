#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "core.h"
#include "cmdline.h"

int qsh_open(shell_t *psh)
{
	if (psh == NULL) return -1;

	memset(psh, 0, sizeof(shell_t));

	psh->user_name = strdup("ZYmelaii");
	psh->group_name = strdup("admin");
	psh->bLogin = 1;

	char buf[MAX_PATH];
	getcwd(buf, sizeof(buf));
	char drive;
	sscanf(buf, "%c:%[^\n]", &drive, psh->workdir + 2);
	char *p = psh->workdir;
	*p++ = '/';
	*p++ = drive + 32;
	do {
		if (*p == '\\')
		{
			*p = '/';
		} else if (*p >= 'A' && *p <= 'Z')
		{
			*p += 32;
		}
	} while (*++p != '\0');

	psh->buf_1 = (char*)malloc(BUF1_SIZE + 1);
	memset(psh->buf_1, 0, BUF1_SIZE + 1);

	psh->bValid = 1;
	return 0;
}

void qsh_close(shell_t *psh)
{
	if (psh == NULL) return;

	if (psh->bLogin == 1)
	{
		free(psh->user_name);
		free(psh->group_name);
		psh->bLogin = 0;
	}

	qsh_reset_cmdline_ptr(psh);
	free(psh->buf_1);

	psh->bValid = 0;
}
