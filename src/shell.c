#include <unistd.h>
#include <string.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "core.h"
#include "cmdline.h"
#include "eval.h"
#include "qshw.h"
#include "uiman.h"

char** qsh_stroke(char **ps, const char *s, const char *delim)
{
	*ps = qsh_strdup(s);

	strtok(*ps, delim);
	int i = 1;
	while (strtok(NULL, delim)) { ++i; }

	char **vs = qsh_malloc(sizeof(char*) * (i + 1));

	strcpy(*ps, s);
	vs[i = 0] = strtok(*ps, delim);
	while (vs[++i] = strtok(NULL, delim));
	vs[i] = NULL;

	return vs;
}

cmd_t* qsh_make_cmd(shell_t *psh, const char *cmdline)
{
	cmd_t *pc = qsh_malloc(sizeof(cmd_t));
	memset(pc, 0, sizeof(cmd_t));

	qsh_cmd_free(psh->prev_cmd);

	pc->root = psh;
	psh->prev_cmd = pc;
	pc->argv = qsh_stroke(&pc->args, cmdline, " ");
	pc->cmd = pc->argv[0];
}

void qsh_cmd_free(cmd_t *pc)
{
	if (pc == NULL) return;

	pc->root->prev_cmd = NULL;

	qsh_free(pc->argv);
	qsh_free(pc->args);
	qsh_free(pc);
}

int _qsh_exec(cmd_t *pc)
{
	_flushall();
	intptr_t hproc = spawnvp(_P_NOWAIT, pc->cmd, pc->argv);
	intptr_t hd = cwait(&pc->ret_code, hproc, NULL);

	if (hd == -1)
	{
		pc->succeed = -1;
		if (errno == ECHILD)
		{
			qshw_print(QSHW_WHITE, "QShell: `%s` command not found.\n", pc->cmd);
		} else if (errno == EINVAL)
		{
			//! IGNORE
		} else
		{
			qshw_print(QSHW_WHITE, "QShell: ");
			qshw_print(QSHW_RED, "unknown error.");
		}
		return (~0);
	} else
	{
		pc->succeed = 1;
		printf("QShell: `%s` exit with %d.\n", pc->cmd, pc->ret_code);
	}
}

int main(int argc, char const *argv[])
{
	qshui_setup();

	shell_t sh;
	qsh_open(&sh);

	while (1)
	{
		qshw_write_prompt(&sh);
		qsh_readline(&sh);
		const char *cmdline = qsh_get_cmdline(&sh);
		if (cmdline != NULL)
		{
			//#- Add eval.. procedure here -
			if (strcmp(cmdline, "exit") == 0)
			{
				break;
			} else if (strcmp(cmdline, "clear") == 0)
			{
				system("cls");
			} else
			{
				qsh_make_cmd(&sh, cmdline);
				_qsh_exec(sh.prev_cmd);
			}
			//#- ENDLINE -
		}
	}

	qsh_close(&sh);

	qshui_exit();

	// QShell should never reach here
	return -1;
}