#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <windows.h>

#include "core.h"
#include "cmdline.h"
#include "eval.h"
#include "qshw.h"
#include "uiman.h"

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
				qshw_print(QSHW_WHITE, "QShell: `%s` command not found.\n", cmdline);
			}
			//#- ENDLINE -
		}
	}

	qsh_close(&sh);

	qshui_exit();

	// QShell should never reach here
	return -1;
}