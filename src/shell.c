#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "core.h"
#include "cmdline.h"
#include "qshw.h"

void qsh_set_signal(int sig, void (*func)(int))
{
	signal(sig, func);
}

int main(int argc, char const *argv[])
{
	qsh_set_signal(SIGINT, SIG_IGN); // ignore ctrl+c

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
				qshw_print(QSHW_WHITE, "QShell: `%s` command not found.\n", cmdline);
			}
			//#- ENDLINE -
		}
		if (sh.read_status_code == -1)
		{
			printf("\n");
		}
	}

	qsh_close(&sh);
	return 0;
}
