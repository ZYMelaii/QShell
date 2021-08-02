#include <stdio.h>

#include <assert.h>
#include <string.h>

#include "core.h"

//! 获取散列桶数量
__inline__ __attribute__((always_inline))
int qsh_hashmap_size(hashmap_t *phm)
{
	return (qsh_msize(phm->data) - sizeof(object_t) * 2) / sizeof(listnode_t);
}

void qsh_hashmap_init(hashmap_t *phm, size_t size)
{
	assert(phm != NULL);

	if (size <= 0 || size > QSH_HASH_MAX)
	{
		phm->data = NULL;
		return;
	}

	size_t real_size = sizeof(listnode_t) * size + sizeof(object_t) * 2;
	phm->data = qsh_malloc(real_size); //! 分配散列桶并追加额外信息
	memset(phm->data, 0, real_size);
}

void qsh_hashmap_free(hashmap_t *phm)
{
	assert(phm != NULL);

	if (phm->data == NULL) return;

	//! 散列桶总数
	size_t size = qsh_hashmap_size(phm);

	listnode_t *llist = (listnode_t*)phm->data;

	int i;
	for (i = 0; i < size; ++i)
	{
		listnode_t *node = llist[i].next;
		while (node != NULL)
		{
			listnode_t *tmp = node->next;
			if (node->obj.data != NULL)
			{
				pair_t *pair = (pair_t*)node->obj.data;
				qsh_free(pair->key);
				if (pair->value != NULL)
				{
					qsh_free(pair->value);
				}
				qsh_free(pair);
			}
			qsh_free(node);
			node = tmp;
		}
		if (llist[i].obj.data != NULL)
		{
			pair_t *pair = (pair_t*)llist[i].obj.data;
			qsh_free(pair->key);
			if (pair->value != NULL)
			{
				qsh_free(pair->value);
			}
			qsh_free(pair);
		}
	}

	qsh_free(phm->data);
	phm->data = NULL;
}

int qsh_hashmap_add(phm, key, hash, cmp, dup)
	hashmap_t *phm; void *key;
	fn_hash_t hash; fn_cmp_t cmp; fn_dup_t dup;
{
	assert(phm != NULL);

	pair_t *pair = qsh_malloc(sizeof(pair_t));

	if (pair == NULL || ((intptr_t)pair & 0xffffffff) == 0xbaadf00d) return -1;

	return_t retcode = 0;

	int bucketid = hash(key) % qsh_hashmap_size(phm);
	listnode_t *node = (listnode_t*)phm->data + bucketid;

	listnode_t *insnode = NULL;
	listnode_t *insnode_parent = NULL;

	if (node->obj.data == NULL) insnode = node;

	if (insnode == NULL)
	{
		while (node)
		{
			if (cmp(key, ((pair_t*)node->obj.data)->key) == 0)
			{
				retcode = 1;
				break;
			}

			if (node->next == NULL)
			{
				insnode = (listnode_t*)qsh_malloc(sizeof(listnode_t));
				if (insnode == NULL)
				{
					retcode = -1;
					break;
				}
				insnode->next = NULL;
				insnode_parent = node;
				insnode_parent->next = insnode;
				break;
			}

			node = node->next;
		}
	}

	if (insnode != NULL && retcode == 0)
	{
		void *dupkey = dup(key);
		if (dupkey != NULL)
		{
			pair->key = dupkey;
			pair->value = NULL;
			insnode->obj.data = (void*)pair;
			return retcode;
		} else
		{
			qsh_free(insnode);
			insnode_parent->next = NULL;
			retcode = -2;
		}
	}

	qsh_free(pair);

	if (retcode == 0) retcode = -3;

	return retcode;
}

void qsh_hashmap_del(phm, key, hash, cmp)
	hashmap_t *phm; void *key;
	fn_hash_t hash; fn_cmp_t cmp;
{
	assert(phm != NULL);

	listnode_t fake_root;

	int bucketid = hash(key) % qsh_hashmap_size(phm);
	fake_root.next = (listnode_t*)phm->data + bucketid;
	listnode_t *node = &fake_root;

	listnode_t *delnode_parent = NULL;

	while (node)
	{
		if (node->next == NULL) return;

		pair_t *pair = (pair_t*)node->next->obj.data;
		
		//! 一个正确地由`qsh_hashmap_add`加入的节点不会产生以下两种情况
		//! 而若是一个根节点，则只可能发生第一种情况
		if (pair == NULL) return;
		if (pair->key == NULL) return;

		if (cmp(key, pair->key) == 0)
		{
			delnode_parent = node;
			break;
		}

		node = node->next;
	}

	//！ 除了不可预料的异常情况，跳出循环后程序总将进入以下条件语句
	if (delnode_parent != NULL)
	{
		node = delnode_parent->next;
		pair_t *pair = (pair_t*)node->obj.data;
		qsh_free(pair->key);
		if (pair->value != NULL)
		{
			qsh_free(pair->value);
		}
		qsh_free(pair);
		pair = NULL;
		if (node == fake_root.next)
		{
			if (node->next != NULL)
			{
				node->obj.data = node->next->obj.data;
				delnode_parent = node;
				node = delnode_parent->next;
			} else
			{
				node->obj.data = NULL;
				return;
			}
		}
		delnode_parent->next = node->next;
		qsh_free(node);
	}
}


/********************************
 *  @author: ZYmelaii
 *  @brief: hashmap_t查找目标节点
 *  @param:
 *     # phm: hashmap_t pointer
 *     # key: key value (not necessarily void*)
 *     # hash: hash function
 *     # cmp: compare function
 *  @note: 
 *  @usage: 
 *  @return:
 *     # NULL: failed
 *     # ...: done
 *******************************/
__inline__ __attribute__((always_inline))
listnode_t* qsh_hashmap_find(phm, key, hash, cmp)
	hashmap_t *phm; void *key;
	fn_hash_t hash; fn_cmp_t cmp;
{
	int bucketid = hash(key) % qsh_hashmap_size(phm);
	listnode_t *node = (listnode_t*)phm->data + bucketid;

	while (node)
	{
		if (cmp(key, ((pair_t*)node->obj.data)->key) == 0)
		{
			return node;
		}
		node = node->next;
	}

	return NULL;
}

void* qsh_hashmap_getval(phm, key, hash, cmp)
	hashmap_t *phm; void *key;
	fn_hash_t hash; fn_cmp_t cmp;
{
	assert(phm != NULL);

	listnode_t *node = qsh_hashmap_find(phm, key, hash, cmp);
	if (node == NULL) return NULL;

	pair_t *pair = (pair_t*)node->obj.data;
	if (pair->value == NULL) return NULL;

	return pair->value;
}

object_t* qsh_hashmap_write(phm, key, hash, cmp)
	hashmap_t *phm; void *key;
	fn_hash_t hash; fn_cmp_t cmp;
{
	assert(phm != NULL);

	listnode_t *node = qsh_hashmap_find(phm, key, hash, cmp);
	if (node == NULL) return NULL;

	size_t size = qsh_msize(phm->data);
	object_t *tmp_objs = (object_t*)((char*)phm->data + size - sizeof(object_t) * 2);
	tmp_objs[0].data = &node->obj;
	tmp_objs[1].data = qsh_malloc(sizeof(object_t));

	return (object_t*)tmp_objs[1].data;
}

void qsh_hashmap_done(hashmap_t *phm)
{
	assert(phm != NULL);

	size_t size = qsh_msize(phm->data);
	object_t *tmp_objs = (object_t*)((char*)phm->data + size - sizeof(object_t) * 2);

	pair_t *pair = (pair_t*)((object_t*)tmp_objs[0].data)->data;

	if (pair->value != NULL)
	{
		qsh_free(pair->value);
		pair->value = NULL;
	}

	pair->value = ((object_t*)tmp_objs[1].data)->data;
	qsh_free(tmp_objs[1].data);
	tmp_objs[1].data = NULL;
}

uint32_t qsh_hash_str(void *key)
{
	const char *str = (const char*)key;
	uint32_t hash = 5381;
	while (*str)
	{
		hash += (hash << 5) + (*str++);
	}
	return (hash & 0x7FFFFFFF);
}