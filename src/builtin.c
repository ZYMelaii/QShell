#pragma once

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <qsh/eval.h>
#include <qsh/qshw.h>

#define NEW_BUILTIN(func, pc) void qsh_builtin_##func(cmd_t *pc)

NEW_BUILTIN(help, pc)
{
	assert(pc != NULL);
	assert(pc->root != NULL);
	assert(pc->root->bValid == 1);

}

NEW_BUILTIN(exit, pc)
{
	assert(pc != NULL);
	assert(pc->root != NULL);
	assert(pc->root->bValid == 1);

	if (pc->argc > 2)
	{
		qshw_xprint("QShell: `exit` too many arguments\n");
	}

	int exitCode = 0;

	if (pc->argc >= 2)
	{
		exitCode = atoi(pc->argv[1]);
		if (exitCode == 0)
		{
			char *p = pc->argv[1];
			int err = 0;
			while (*p != '\0')
			{
				if (*p++ != '0')
				{
					err = 1;
					break;
				}
			}
			if (err)
			{
				qshw_xprint("QShell: `exit` passing invalid exit code (%s)\n", pc->argv[1]);
				exitCode = -1;
			}
		}
	}

	qshui_exit(exitCode);
}

NEW_BUILTIN(echo, pc)
{
	assert(pc != NULL);
	assert(pc->root != NULL);
	assert(pc->root->bValid == 1);

}

NEW_BUILTIN(clear, pc)
{
	assert(pc != NULL);
	assert(pc->root != NULL);
	assert(pc->root->bValid == 1);

	system("cls");
}

NEW_BUILTIN(cd, pc)
{
	assert(pc != NULL);
	assert(pc->root != NULL);
	assert(pc->root->bValid == 1);

}

NEW_BUILTIN(pwd, pc)
{
	assert(pc != NULL);
	assert(pc->root != NULL);
	assert(pc->root->bValid == 1);

}