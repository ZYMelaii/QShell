#include "core.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
	listnode_t *node = (listnode_t*)0x156110;
	printf("[ 0x%016x ]\n", node->obj.data);
	return 0;
}