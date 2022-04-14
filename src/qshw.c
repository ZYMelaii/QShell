#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <windows.h>

#include <qsh/qshw.h>

void qshw_vprint(int color, const char *format, va_list args)
{

	if (strlen(format) == 0) return;

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

const char* qshw_match_format_token(const char *s, int *t_spec, int *t_len)
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

	*t_spec = -1;
	*t_len = 0;

	if (*s != '%')
	{	//! 无效格式符
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
	*t_len = 0;
	return s;
}

void qshw_xprint(const char *format, ...)
{	//@ 未检查安全性
	va_list v, tmp;
	va_start(v, format);
	va_copy(tmp, v);

	int color = 030;

	char *s = qsh_strdup(format);
	char *p = s; //! 字符串起始指针
	char *q = p; //! 字符串末尾指针

	int b_quit = 0;
	while (1)
	{
		if (*q == '\x02' || *q == '\0')
		{	//! 输出前一轮的字符串并更新颜色代码
			if (*q == '\0') b_quit = 1;
			*q = '\0';

			qshw_vprint(color & 007, p, v);
			va_end(v);
			va_copy(v, tmp);

			if (b_quit) break;

			color = (int)*++q;
			p = ++q;
		}

		if (*q == '%')
		{
			int t_spec = -1, t_len = 0;
			q = qshw_match_format_token(q, &t_spec, &t_len);
			if (t_spec == -1)
			{	//! 假定格式符是有效的，不处理该情况

			} else if (t_spec == 0)
			{	//! %%

			} else
			{
				switch (t_spec)
				{
					case 1: //! d i
					{
						switch (t_len)
						{
							case 1: { va_arg(tmp, short int); break; } //! h
							case 2: { va_arg(tmp, signed char); break; } //! hh
							case 3: { va_arg(tmp, long int); break; } //! l
							case 4: { va_arg(tmp, long long int); break; } //! ll
							case 5: { va_arg(tmp, intmax_t); break; } //! j
							case 6: { va_arg(tmp, size_t); break; } //! z
							case 7: { va_arg(tmp, ptrdiff_t); break; } //! t
							default:  { va_arg(tmp, int); break; } //! (none) L
						}
						break;
					}
					case 2: //! u o x X
					{
						switch (t_len)
						{
							case 1: { va_arg(tmp, unsigned short int); break; } //! h
							case 2: { va_arg(tmp, unsigned char); break; } //! hh
							case 3: { va_arg(tmp, unsigned long int); break; } //! l
							case 4: { va_arg(tmp, unsigned long long int); break; } //! ll
							case 5: { va_arg(tmp, uintmax_t); break; } //! j
							case 6: { va_arg(tmp, size_t); break; } //! z
							case 7: { va_arg(tmp, ptrdiff_t); break; } //! t
							default:  { va_arg(tmp, unsigned int); break; } //! (none) L
						}
						break;
					}
					case 3: //! f F e E g G a A
					{
						switch (t_len)
						{
							case 8: { va_arg(tmp, long double); break; } //! L
							default:  { va_arg(tmp, double); break; } //! (none) h hh l ll j z t
						}
						break;
					}
					case 4: //! c
					{
						switch (t_len)
						{
							case 3: { va_arg(tmp, wint_t); break; } //! l
							default:  { va_arg(tmp, int); break; } //! (none) h hh ll j z t L
						}
						break;
					}
					case 5: //! s
					case 6: //! p
					case 7: //! n
					{
						switch (t_len)
						{
							default:  { va_arg(tmp, void*); break; } //! (none) h hh l ll j z t L
						}
						break;
					}
				}
			}
		}

		++q;
	}

	va_end(tmp);

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

	qshw_xprint("\x02\032%s@%s:\x02\033%s\x02\030$ ", psh->user_name, psh->group_name, psh->workdir);

	fflush(stdout);
}