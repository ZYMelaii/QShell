extern void test_0();
extern void test_1();
extern void test_2();
extern void test_3();

typedef void(*T)();

T TEST_CASES[] =
{
	test_0, test_1, test_2, test_3, //! test.0.c
	0L,
};

int main(int argc, char const *argv[])
{
	T *TEST_CASE = TEST_CASES;
	while (*TEST_CASE != 0L)
	{
		(*TEST_CASE)();
		++TEST_CASE;
	}
	return 0;
}