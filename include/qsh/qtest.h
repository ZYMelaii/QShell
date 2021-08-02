#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>

#ifdef QSH_TEST

#define TEST_TIMER_NEW(timer)              \
	union _LARGE_INTEGER timer##_start;    \
	double timer##_total = .0;             \
	double timer##_freq;                   \
	do {                                   \
		LARGE_INTEGER f;                   \
		QueryPerformanceFrequency(&f);     \
	 	timer##_freq = (double)f.QuadPart; \
	 } while (0);

#define TEST_TIMER_START(timer)                  \
	do {                                         \
		QueryPerformanceCounter(&timer##_start); \
	} while (0);

#define TEST_TIMER_STOP(timer)                                                 \
	do {                                                                       \
		union _LARGE_INTEGER t;                                                \
		QueryPerformanceCounter(&t);                                           \
		timer##_total += (t.QuadPart - timer##_start.QuadPart) / timer##_freq; \
	} while (0);

#define TEST_TIMER_CLEAR(timer) \
	do {                        \
		timer##_total = .0;     \
	} while (0);

#define TEST_TIMER_GETVAL(timer) ((timer##_total) * 1000000)

#define TEST_TMPAUSE               \
	do {                           \
		TEST_TIMER_STOP(_test_tm); \
	} while (0);

#define TEST_TMRUN                  \
	do {                            \
		TEST_TIMER_START(_test_tm); \
	} while (0);

#define TEST_INFO(str, args...)                         \
do {                                                    \
	TEST_TMPAUSE;                                       \
	time_t tm;                                          \
	struct tm *now;                                     \
	time(&tm);                                          \
	now = localtime(&tm);                               \
	printf("%d-%02d-%02d %02d:%02d:%02d - [INFO@%s] "   \
		str "\n",                                       \
		now->tm_year + 1900, now->tm_mon, now->tm_mday, \
		now->tm_hour, now->tm_min, now->tm_sec,         \
		_test_name, ##args);                            \
	TEST_TMRUN;                                         \
} while (0);

#define TEST_BEGIN(target)            \
do {                                  \
	const char *_test_name = #target; \
	size_t _test_total = 0;           \
	size_t _test_done  = 0;           \
	size_t _test_failed = 0;          \
	size_t _test_unitsize = 100;      \
	TEST_TIMER_NEW(_test_tm);         \
	TEST_TIMER_START(_test_tm);

#define TEST_END                                          \
	do {                                                  \
		_test_total = _test_done + _test_failed;          \
		TEST_TIMER_STOP(_test_tm);                        \
		double t = TEST_TIMER_GETVAL(_test_tm) * 0.001;   \
		double t_per_unit;                                \
		size_t total = _test_total > 0 ? _test_total : 1; \
		t_per_unit = t * _test_unitsize / total;          \
		TEST_INFO("total=%d; done=%d; failed=%d; "        \
			"cost(total/per %d)=%.2fms/%.4fms",           \
			_test_total, _test_done, _test_failed,        \
			_test_unitsize, t, t_per_unit);               \
		TEST_TIMER_CLEAR(_test_tm);                       \
	} while (0);                                          \
} while (0);

#define TEST_DONE     \
	do {              \
		++_test_done; \
	} while (0);

#define TEST_FAILED(expr)                               \
	do {                                                \
		++_test_failed;                                 \
		TEST_INFO("test failed; target=\"%s\"", #expr); \
	} while (0);

#define TEST(statement) \
	do {                \
		statement;      \
		TEST_DONE;      \
	} while (0);

#define TEST_EXPR(expr)    \
	if ((expr))            \
	{                      \
		TEST_DONE;         \
	} else                 \
	{                      \
		TEST_FAILED(expr); \
	}

#define TEST_EQU(expr, value)       \
	if ((expr) == value)            \
	{                               \
		TEST_DONE;                  \
	} else                          \
	{                               \
		TEST_FAILED(expr == value); \
	}

#define TEST_NEQ(expr, value)       \
	if ((expr) != value)            \
	{                               \
		TEST_DONE;                  \
	} else                          \
	{                               \
		TEST_FAILED(expr != value); \
	}

#else

#define TEST_TIMER_NEW(timer)

#define TEST_TIMER_START(timer)

#define TEST_TIMER_STOP(timer)

#define TEST_TIMER_CLEAR(timer)

#define TEST_TIMER_GETVAL(timer)

#define TEST_TMPAUSE

#define TEST_TMRUN

#define TEST_INFO(str, agrs...)

#define TEST_BEGIN(target)

#define TEST_END

#define TEST_DONE

#define TEST_FAILED(expr)

#define TEST(statement)

#define TEST_EXPR(expr)

#define TEST_EQU(expr, value)

#define TEST_NEQ(expr, value)

#endif /* QSH_TEST */