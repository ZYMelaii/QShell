#include <malloc.h>
#include <string.h>

#include "core.h"

void* qsh_malloc(size_t size)
{
	return malloc(size);
}

void qsh_free(void *ptr)
{
	free(ptr);
}

size_t qsh_msize(void *ptr)
{
	return _msize(ptr);
}

char* qsh_strdup(const char *s)
{
	size_t len = strlen(s) + 1;
	void *dup = qsh_malloc(len);
	if (dup == NULL) return NULL;
	return (char*)memcpy(dup, s, len);
}