#include <string.h>

#include "envr.h"

__inline__ __attribute__((always_inline))
int qsh_hashmap_sscmp(void *x, void *y)
{
	return strcmp((const char*)x, (const char*)y);
}

__inline__ __attribute__((always_inline))
void* qsh_hashmap_ssdup(const void *src)
{
	if (src == NULL) return NULL;
	return (void*)qsh_strdup((const char*)src);
}

return_t qsh_var_new(context_t *ctx, const char *key)
{
	return_t retcode = qsh_hashmap_add((hashmap_t*)ctx->symmap, key,
		qsh_hash_str, qsh_hashmap_sscmp, qsh_hashmap_ssdup);
	return retcode;
}

return_t qsh_var_del(context_t *ctx, const char *key)
{
	qsh_hashmap_del((hashmap_t*)ctx->symmap, key, qsh_hash_str, qsh_hashmap_sscmp);
	const void *pcheck = qsh_hashmap_getval((hashmap_t*)ctx->symmap, key,
		qsh_hash_str, qsh_hashmap_sscmp);
	return (pcheck != NULL ? 0 : -1);
}

return_t qsh_var_get(context_t *ctx, const char *key, const char **pvalue)
{
	*pvalue = (const char*)qsh_hashmap_getval((hashmap_t*)ctx->symmap, key,
		qsh_hash_str, qsh_hashmap_sscmp);
	return (*pvalue == BADPTR ? 1 : (*pvalue == NULL ? -1 : 0));
}

return_t qsh_var_set(context_t *ctx, const char *key, const char *value)
{
	void *valdup = (void*)qsh_hashmap_ssdup(value);
	if (valdup == NULL) return -2;
	object_t *obj = qsh_hashmap_write((hashmap_t*)ctx->symmap, key,
		qsh_hash_str, qsh_hashmap_sscmp);
	if (obj != NULL)
	{
		obj->data = valdup;
		qsh_hashmap_done((hashmap_t*)ctx->symmap);
		return 0;
	}
	return -1;
}