//! target=qshw_match_format_token

#define QSH_TEST
#include <qsh/qtest.h>

extern const char* qshw_match_format_token(const char *s, int *t_spec, int *t_len);

void test_0()
{
	const size_t N = 4;

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

	TEST_BEGIN(QShell Winform - function qshw_match_format_token);

	int i;
	for (i = 0; i < N; ++i)
	{
		int t_spec, t_len;
		qshw_match_format_token(s_test[i], &t_spec, &t_len);
		TEST_EXPR(t_spec == i_result[i][0] && t_len == i_result[i][1]);
	}

	TEST_END;
}