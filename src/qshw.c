#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <windows.h>

#include "qshw.h"

void qshw_print(int color, const char *format, ...)
{
	WORD colorCode[] = {
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // white
		FOREGROUND_RED, // red
		FOREGROUND_GREEN, // green
		FOREGROUND_BLUE, // blue
		FOREGROUND_RED | FOREGROUND_GREEN, // yellow
		FOREGROUND_RED | FOREGROUND_BLUE, // pink
		FOREGROUND_GREEN | FOREGROUND_BLUE, // cyan
	};
	const int colorNum = sizeof(colorCode) / sizeof(WORD);


	if (color < 0 || color >= colorNum)
	{
		color = 0;
	}

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (handle == NULL)
	{
		return;
	}

	SetConsoleTextAttribute(handle, colorCode[color]);

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	SetConsoleTextAttribute(handle, colorCode[0]);
}

void qshw_write_prompt(shell_t *psh)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);

	qshw_print(QSHW_GREEN, "%s@%s:", psh->user_name, psh->group_name);
	qshw_print(QSHW_BLUE, psh->workdir);
	qshw_print(QSHW_WHITE, "$ ");

	fflush(stdout);
}