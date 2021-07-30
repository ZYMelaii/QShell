#include <stdio.h>
#include <stdarg.h>

extern const char* qshw_match_format_token(const char*, int*, int*); //! `qshw.c`

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
