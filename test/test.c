#include <stdio.h>
#include <stdarg.h>

const char* qshw_match_format_token(const char *s, int *t_spec, int *t_len)
{	//! 返回一个完整token的末尾，本函数假定给定字符串是有效的。
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

	while (!((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z')) && *p != '\0') { ++p; }

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

int main(int argc, char const *argv[])
{
	const int cnt = 4;
	char* s_test[] =
	{
		"%%",
		"%d",
		"%.6llf",
		"%08x",
	};
	int i_result[][2] =
	{
		{0, 0},
		{1, 0},
		{3, 4},
		{2, 0},
	};
	int i;
	printf("TEST@qshw_match_format_token:\n");
	for (i = 0; i < cnt; ++i)
	{
		int t_spec, t_len;
		qshw_match_format_token(s_test[i], &t_spec, &t_len);
		if (t_spec == i_result[i][0] && t_len == i_result[i][1])
		{
			printf("Passed.\n");
		} else
		{
			printf("Failed: `%s` <- (%d, %d); correct: (%d, %d)\n",
				s_test[i], t_spec, t_len, i_result[i][0], i_result[i][1]);
		}
	}
	return 0;
}
