#pragma once

#include <unistd.h>
#include <string.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <qsh/core.h>
#include <qsh/cmdline.h>
#include <qsh/eval.h>
#include <qsh/qshw.h>
#include <qsh/uiman.h>

char** qsh_stroke(char **ps, int *num, const char *s, const char *delim)
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

	*num = i;

	return vs;
}

cmd_t* qsh_make_cmd(shell_t *psh, const char *cmdline)
{
	cmd_t *pc = qsh_malloc(sizeof(cmd_t));
	memset(pc, 0, sizeof(cmd_t));

	qsh_cmd_free(psh->prev_cmd);

	pc->root = psh;
	psh->prev_cmd = pc;
	pc->argv = qsh_stroke(&pc->args, &pc->argc, cmdline, " ");
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
			// qshw_print(QSHW_WHITE, "QShell: `%s` command not found.\n", pc->cmd);
			qshw_xprint("QShell: \x02\031`%s` \x02\030command not found.\n", pc->cmd);
			// qshw_xprint("S: \x02\001[ERROR] \x02\000this is a trick.\n");
		} else if (errno == EINVAL)
		{
			//! IGNORE
		} else
		{
			qshw_xprint("QShell: \x02\031[ERROR] \x02\030unknown error\n");
		}
		return (~0);
	} else
	{
		pc->succeed = 1;
		qshw_xprint("QShell: `%s` exit with %d.\n", pc->cmd, pc->ret_code);
	}
}

void qsh_mainloop(shell_t *psh)
{
	while (1)
	{
		qshw_write_prompt(psh);
		qsh_readline(psh);
		const char *cmdline = qsh_get_cmdline(psh);
		if (cmdline != NULL)
		{
			cmd_t *pc = qsh_make_cmd(psh, cmdline);
			if (strcmp(pc->cmd, "exit") == 0)
			{
				qsh_builtin_exit(pc);
			} else if (strcmp(pc->cmd, "clear") == 0)
			{
				qsh_builtin_clear(pc);
			} else
			{
				_qsh_exec(pc);
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	qshui_setup(QSH_CUI);

	shell_t sh;
	qsh_open(&sh);

	qsh_mainloop(&sh);

	qsh_close(&sh);

	qshui_exit(0);

	// QShell should never reach here
	return -1;
}