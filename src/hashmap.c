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

__inline__ __attribute__((always_inline))
void qsh_hashmap_add_impl(listnode_t *node, void *dupkey)
{
	node->obj.data = qsh_malloc(sizeof(pair_t));
	((pair_t*)node->obj.data)->key = dupkey;
	((pair_t*)node->obj.data)->value = NULL;
}

int qsh_hashmap_add(phm, key, hash, cmp, dup)
	hashmap_t *phm; void *key;
	fn_hash_t hash; fn_cmp_t cmp; fn_dup_t dup;
{
	assert(phm != NULL);

	int bucketid = hash(key) % qsh_hashmap_size(phm);
	listnode_t *node = (listnode_t*)phm->data + bucketid;

	if (node->obj.data == NULL)
	{
		qsh_hashmap_add_impl(node, dup(key));
		return 0;
	}

	while (node)
	{
		if (cmp(key, ((pair_t*)node->obj.data)->key) == 0)
		{	//! 键已经存在
			return 1;
		}

		if (node->next == NULL)
		{
			node->next = (listnode_t*)qsh_malloc(sizeof(listnode_t));
			qsh_hashmap_add_impl(node->next, dup(key));
			return 0;
		}

		node = node->next;
	}

	return -1;
}

__inline__ __attribute__((always_inline))
void qsh_hashmap_del_impl(listnode_t *node)
{
	pair_t *pair = (pair_t*)node->obj.data;
	qsh_free(pair->key);
	if (pair->value != NULL)
	{
		qsh_free(pair->value);
	}
	qsh_free(pair);
	node->obj.data = NULL;
}

void qsh_hashmap_del(phm, key, hash, cmp)
	hashmap_t *phm; void *key;
	fn_hash_t hash; fn_cmp_t cmp;
{
	assert(phm != NULL);

	int bucketid = hash(key) % qsh_hashmap_size(phm);
	listnode_t *node = (listnode_t*)phm->data + bucketid;

	if (cmp(key, ((pair_t*)node->obj.data)->key) == 0)
	{
		qsh_hashmap_del_impl(node);
		if (node->next != NULL)
		{
			node->obj.data = node->next->obj.data;
			listnode_t *tmp = node->next;
			node->next = node->next->next;
			qsh_free(tmp);
		}
		return;
	}

	while (node)
	{
		if (node->next == NULL) return;

		if (cmp(key, ((pair_t*)node->next->obj.data)->key) == 0)
		{
			qsh_hashmap_del_impl(node->next);
			listnode_t *tmp = node->next;
			node->next = node->next->next;
			qsh_free(tmp);
			return;
		}

		node = node->next;
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