#include <unistd.h>
#include <string.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

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
				char *s;
				char **_argv = qsh_stroke(&s, cmdline, " ");

				_flushall();

				intptr_t hproc = spawnvp(_P_WAIT, _argv[0], _argv);

				if (hproc == (~0))
				{
					qshw_print(QSHW_WHITE, "QShell: `%s` command not found.\n", _argv[0]);
				} else
				{
					int termstat;
					cwait(&termstat, hproc, _WAIT_CHILD);
				}

				qsh_free(_argv);
				qsh_free(s);
			}
			//#- ENDLINE -
		}
	}

	qsh_close(&sh);

	qshui_exit();

	// QShell should never reach here
	return -1;
}
