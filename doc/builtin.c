#include <assert.h>

#include "eval.h"

#define NEW_BUILTIN(func, psh, pc) void qsh_builtin_##func(shell_t *psh, cmd_t *pc)

NEW_BUILTIN(help, psh, pc)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);
	assert(pc != NULL);

}
NEW_BUILTIN(exit, psh, pc)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);
	assert(pc != NULL);

}

NEW_BUILTIN(echo, psh, pc)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);
	assert(pc != NULL);

}

NEW_BUILTIN(clear, psh, pc)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);
	assert(pc != NULL);

}

NEW_BUILTIN(cd, psh, pc)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);
	assert(pc != NULL);

}

NEW_BUILTIN(pwd, psh, pc)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);
	assert(pc != NULL);

}