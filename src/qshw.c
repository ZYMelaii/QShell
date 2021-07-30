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

const char* qshw_match_format_token(const char *s, int *t_len, int *t_spec)
{	//! 返回一个完整token的末尾，本函数假定给定字符串是有效的。
	/********************************
	 * ┌────────┬───────────────────────────────────────────────────────────────────────────────────────────────────────┐
	 * │        │                                              specifiers                                               │
	 * ├────────┼───────────────┬────────────────────────┬─────────────────┬────────┬──────────┬───────┬────────────────┤
	 * │ length │ d i           │ u o x X                │ f F e E g G a A │ c      │ s        │ p     │ n              │
	 * ├────────┼───────────────┼────────────────────────┼─────────────────┼────────┼──────────┼───────┼────────────────┤
	 * │ (none) │ int           │ unsigned int           │ double          │ int    │ char*    │ void* │ int*           │
	 * │ h      │ short int     │ unsigned short int     │                 │        │          │       │ short int*     │
	 * │ hh     │ signed char   │ unsigned char          │                 │        │          │       │ signed char*   │
	 * │ l      │ long int      │ unsigned long int      │                 │ wint_t │ wchar_t* │       │ long int*      │
	 * │ ll     │ long long int │ unsigned long long int │                 │        │          │       │ long long int* │
	 * │ j      │ intmax_t      │ uintmax_t              │                 │        │          │       │ intmax_t*      │
	 * │ z      │ size_t        │ size_t                 │                 │        │          │       │ size_t*        │
	 * │ t      │ ptrdiff_t     │ ptrdiff_t              │                 │        │          │       │ ptrdiff_t*     │
	 * │ L      │               │                        │ long double     │        │          │       │                │
	 * └────────┴───────────────┴────────────────────────┴─────────────────┴────────┴──────────┴───────┴────────────────┘
	 *******************************/

	*t_len = 0;

	if (*s != '%')
	{	//! 无效格式符
		*t_spec = -1;
		return s;
	}

	const char *p = s;
	if (p[1] == '%')
	{	//! %%
		*t_spec = 0;
		return p + 1;
	}

	while (!(*p >= 'a' && *p <= 'z' || *p >= 'A' && *p <= 'Z') && *p != '\0') { ++p; }

	if (*p == '\0')
	{	//! 无效格式符
		*t_spec = -1;
		return s;
	}

	int i = 0;
	while (i++ < 3)
	{
		switch (*p)
		{
			case 'd': case 'i': { *t_spec = 1; break; }
			case 'u': case 'o': case 'x': case 'X': { *t_spec = 2; break; }
			case 'f': case 'F': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A': { *t_spec = 3; break; }
			case 'c': { *t_spec = 4; break; }
			case 's': { *t_spec = 5; break; }
			case 'p': { *t_spec = 6; break; }
			case 'n': { *t_spec = 7; break; }
		}

		if (*t_spec > 0)
		{
			return p;
		}

		switch (*p)
		{
			case 'h': { *t_len = *t_len == 1 ? 2 : 1; break; }
			case 'l': { *t_len = *t_len == 3 ? 4 : 3; break; }
			case 'j': { *t_len = 5; break; }
			case 'z': { *t_len = 6; break; }
			case 't': { *t_len = 7; break; }
			case 'L': { *t_len = 8; break; }
		}

		++p;
	}

	*t_spec = -1;
	return s;
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
