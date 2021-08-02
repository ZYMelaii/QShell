//! gcc hashmap.c mmem.c hashmap_test.c -o test -D_DEBUG -I..\include
//! target=hashmap_t

#include "qtest.h"

#include <qsh/core.h>

#include <string.h>

void setup()
{
	srand((unsigned)time(NULL));
}

char* genstr(int maxlen)
{	//! 生成随机字符串

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

	setup();

	const size_t N = 1000000, M = 4096, T = 128;
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
		int cnt = 0;
		hashmap_t map;
		qsh_hashmap_init(&map, 256);

		TEST_TMPAUSE;
		char *strs[T];
		for (j = 0; j < T; ++j)
		{
			strs[j] = genstr(64);
		}
		TEST_TMRUN;

		size_t U = 1;
		while (U <= T)
		{
			for (i = 0; i < M; ++i)
			{
				for (j = 0; j < U; ++j)
				{
					TEST_NEQ(qsh_hashmap_add(&map, (void*)strs[j], qsh_hash_str, sscmp, sdup), -1);
				}

				// TEST_TMPAUSE;
				// {	//! 检查哈希表有效性
				// 	hashmap_t *phm = &map;
				// 	listnode_t *lists = (listnode_t*)phm->data;
				// 	size_t num = (qsh_msize(phm->data) - sizeof(object_t) * 2) / sizeof(listnode_t);
				// 	int k;
				// 	for (k = 0; k < num; ++k)
				// 	{
				// 		listnode_t *node = lists + k;
				// 		printf("[%04d] ", k + 1);
				// 		while (node)
				// 		{
				// 			printf ("0x%016x -> ", node->obj.data);
				// 			node = node->next;
				// 		}
				// 		printf("null\n");
				// 	}
				// }
				// TEST_TMRUN;

				for (j = 0; j < U; ++j)
				{
					TEST(qsh_hashmap_del(&map, (void*)strs[j], qsh_hash_str, sscmp));
				}
			}
			U *= 2;
		}

		TEST_TMPAUSE;
		for (j = 0; j < T; ++j)
		{
			qsh_free(strs[j]);
		}
		TEST_TMRUN;

		qsh_hashmap_free(&map);
	}
	TEST_END;

	return 0;
}