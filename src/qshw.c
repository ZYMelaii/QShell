#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <windows.h>

#include "qshw.h"

void qshw_vprint(int color, const char *format, va_list args)
{
	WORD colorCode[] = {
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // white
		FOREGROUND_RED, // red
		FOREGROUND_GREEN, // green
		FOREGROUND_BLUE, // blue
		FOREGROUND_RED | FOREGROUND_GREEN, // yellow
		FOREGROUND_RED | FOREGROUND_BLUE, // magenta
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

	vprintf(format, args);

	SetConsoleTextAttribute(handle, colorCode[0]);
}

void qshw_print(int color, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	qshw_vprint(color, format, args);
	va_end(args);
}

void qshw_xprint(const char *format, ...)
{	//@ 未检查安全性
	// "...\x02\000...\x02..."
	// "设定颜色|颜色代码|目标字符串|设定颜色结束";
	va_list v, tmp;
	va_start(tmp, format);

	int color = 0;

	char *s = qsh_strdup(format);
	char *p = s, *q = p;
	while (1)
	{
		if (*q == '\x02')
		{
			*q = '\0';
			if (p == s)
			{	//! 开头字符未设置颜色，默认按白色输出
				va_copy(v, tmp);
				qshw_vprint(color, p, v);
			}
			color = *++q;
			p = ++q;
		}
	}
	qsh_free(s);
}

void qshw_write_prompt(shell_t *psh)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);

	if (psh->read_status_code == -1)
	{
		printf("\n");
	}

	qshw_print(QSHW_GREEN, "%s@%s:", psh->user_name, psh->group_name);
	qshw_print(QSHW_BLUE, psh->workdir);
	qshw_print(QSHW_WHITE, "$ ");

	fflush(stdout);
}