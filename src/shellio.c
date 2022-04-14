#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <qsh/core.h>
#include <qsh/cmdline.h>

// uiman.c
extern void qshui_set_global_ptr(void*);

int qsh_open(shell_t *psh)
{
	if (psh == NULL) return -1;

	memset(psh, 0, sizeof(shell_t));

	psh->user_name = qsh_strdup("ZYmelaii");
	psh->group_name = qsh_strdup("localhost");
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

	psh->buf_1 = (char*)qsh_malloc(BUF1_SIZE + 1);
	memset(psh->buf_1, 0, BUF1_SIZE + 1);

	psh->bValid = 1;

	qshui_set_global_ptr(psh);

	return 0;
}

void qsh_close(shell_t *psh)
{
	if (psh == NULL) return;
	if (psh->bValid == 0) return;

	if (psh->bLogin == 1)
	{
		qsh_free(psh->user_name);
		qsh_free(psh->group_name);
		psh->bLogin = 0;
	}

	qsh_reset_cmdline_ptr(psh);
	qsh_free(psh->buf_1);

	psh->bValid = 0;
}