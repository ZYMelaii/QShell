#include "qtest.h"

//! target=qshw_match_format_token [from `qshw.c`]
extern const char* qshw_match_format_token(const char *s, int *t_spec, int *t_len);

int main(int argc, char const *argv[])
{
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

	const int cnt = 4;

	int i;

	TEST_BEGIN(qshw_match_format_token);

	for (i = 0; i < cnt; ++i)
	{
		int t_spec, t_len;
		qshw_match_format_token(s_test[i], &t_spec, &t_len);
		TEST_EXPR(t_spec == i_result[i][0] && t_len == i_result[i][1]);
	}

	TEST_END;

	return 0;
}