//! target=hashmap_t

#define QSH_TEST

#include <qsh/qtest.h>
#include <qsh/core.h>

#include <string.h>

void setup()
{
	srand((unsigned)time(NULL));
}

char* genstr(int maxlen)
{	//! 生成随机字符串
	srand((unsigned)time(NULL));

	int len = (rand() % maxlen) + 1;
	char *str = (char*)qsh_malloc(len + 1);
	if (str == NULL) return NULL;
 
 	int i;
	for (i = 0; i < len; ++i)
	{
		switch (rand() % 4)
		{
			case 0: str[i] = 'A' + rand() % 26; break;
			case 1: str[i] = 'a' + rand() % 26; break;
			case 2: str[i] = '0' + rand() % 10; break;
			case 3:
			default: str[i] = '_'; break;
		}
	}

	str[len] = '\0';

	return str;
}

int sscmp(void *x, void *y)
{
	if (x == NULL || y == NULL) return -1;
	return strcmp((const char*)x, (const char*)y);
}

void* sdup(const void *src)
{
	return (void*)qsh_strdup((const char*)src);
}

void test_0()
{
	setup();

	TEST_BEGIN(QShell hashmap - Construct & Destruct);

	size_t i;

	const size_t N = 1000000;

	int n = 0;

	for (i = 0; i < N; ++i)
	{
		hashmap_t map;
		qsh_hashmap_init(&map, 1 << (++n % 11));
		TEST_NEQ(map.data, NULL);
		qsh_hashmap_free(&map);
		TEST_EQU(map.data, NULL);
	}

	TEST_END;
}

void test_1()
{
	setup();

	TEST_BEGIN(QShell hashmap - Add & Delete Key);

	size_t i, j;

	const size_t N = 10000, M = 1024;
	const size_t T = 256;

	hashmap_t map;
	qsh_hashmap_init(&map, T);

	TEST_TMPAUSE;
	char *strs[T];
	for (i = 0; i < T;) { strs[i++] = genstr(64); }
	TEST_TMRUN;

	size_t U = 1;
	while (U <= M)
	{
		for (i = 0; i < N; ++i)
		{
			for (j = 0; j < U; ++j)
			{
				TEST_EXPR(qsh_hashmap_add(&map, (void*)strs[j % T], qsh_hash_str, sscmp, sdup) >= 0);
			}
			for (j = 0; j < U; ++j)
			{
				TEST(qsh_hashmap_del(&map, (void*)strs[j % T], qsh_hash_str, sscmp));
			}
		}
		U <<= 1;
	}

	TEST_TMPAUSE;
	for (i = 0; i < T;) { qsh_free(strs[i++]); }
	TEST_TMRUN;

	qsh_hashmap_free(&map);

	TEST_END;
}

void test_2()
{
	setup();

	TEST_BEGIN(QShell hashmap - Find Element);

	size_t i, j;

	const size_t N = 1000000, M = 10;
	const size_t T = 256, G = T * 4;

	hashmap_t map;
	qsh_hashmap_init(&map, T);

	TEST_TMPAUSE;
	char *keys[G];
	for (i = 0; i < G; ++i)
	{
		keys[i] = genstr(64);
		qsh_hashmap_add(&map, (void*)keys[i], qsh_hash_str, sscmp, sdup);
	}
	TEST_TMRUN;

	j = 1;
	for (i = 0; i < N * M; ++i)
	{
		j = (j * M) % G;
		TEST_EQU(qsh_hashmap_getval(&map, keys[j], qsh_hash_str, sscmp), ((void*)0xffffffff));
	}

	TEST_TMPAUSE;
	for (i = 0; i < G; ++i)
	{
		qsh_hashmap_del(&map, (void*)keys[i], qsh_hash_str, sscmp);
		qsh_free(keys[i]);
	}
	TEST_TMRUN;

	qsh_hashmap_free(&map);

	TEST_END;
}

void test_3()
{
	setup();

	TEST_BEGIN(QShell hashmap - Read & Write);

	size_t i, j;

	const size_t N = 10000000, M = 137;
	const size_t T = 256, G = T * 4;

	hashmap_t map;
	qsh_hashmap_init(&map, T);

	TEST_TMPAUSE;
	char *keys[G];
	for (i = 0; i < G; ++i)
	{
		keys[i] = genstr(64);
		qsh_hashmap_add(&map, (void*)keys[i], qsh_hash_str, sscmp, sdup);
	}
	TEST_TMRUN;

	j = 1;
	for (i = 0; i < N; ++i)
	{
		j = (j * M) % G;
		object_t *obj = qsh_hashmap_write(&map, keys[j], qsh_hash_str, sscmp);
		TEST_TMPAUSE;
		void *val = (void*)genstr(256);
		TEST_TMRUN;
		obj->data = val;
		qsh_hashmap_done(&map);
		TEST_EQU(sscmp(val, qsh_hashmap_getval(&map, keys[j], qsh_hash_str, sscmp)), 0);
	}

	TEST_TMPAUSE;
	for (i = 0; i < G; ++i)
	{
		qsh_hashmap_del(&map, (void*)keys[i], qsh_hash_str, sscmp);
		qsh_free(keys[i]);
	}
	TEST_TMRUN;

	qsh_hashmap_free(&map);

	TEST_END;
}