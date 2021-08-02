//! target=hashmap_t

#include "qtest.h"

#include <qsh/core.h>

#include <string.h>

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
	return strcmp((const char*)x, (const char*)y);
}

void* sdup(const void *src)
{
	return (void*)qsh_strdup((const char*)src);
}

int main(int argc, char const *argv[])
{
	const size_t N = 1000000, M = 4096;
	size_t i, j;

	TEST_BEGIN(QHashmap - Construct & Destruct);
	{
		size_t szTable[11] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, };
		int n = 0;
		for (i = 0; i < N; ++i)
		{
			hashmap_t map;
			qsh_hashmap_init(&map, szTable[++n % 11]);
			TEST_NEQ(map.data, NULL);
			qsh_hashmap_free(&map);
			TEST_EQU(map.data, NULL);
		}
	}
	TEST_END;

	TEST_BEGIN(QHashmap - Add & Delete Key);
	{
		hashmap_t map;
		qsh_hashmap_init(&map, 1024);
		for (i = 0; i < N; ++i)
		{
			TEST_TMPAUSE;
			char *strs[M];
			for (j = 0; j < M; ++j)
			{
				strs[j] = genstr(64);
			}
			TEST_TMRUN;

			for (j = 0; j < M; ++j)
			{ 
				TEST_NEQ(qsh_hashmap_add(&map, (void*)strs[j], qsh_hash_str, sscmp, sdup), -1);
			}

			for (j = 0; j < M; ++j)
			{
				TEST(qsh_hashmap_del(&map, (void*)strs[j], qsh_hash_str, sscmp));
			}

			TEST_TMPAUSE;
			for (j = 0; j < M; ++j)
			{
				 qsh_free(strs[j]);
			}
			TEST_TMRUN;
		}
		qsh_hashmap_free(&map);
	}
	TEST_END;

	return 0;
}