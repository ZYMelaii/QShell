#ifndef QSHELL_CORE_H
#define QSHELL_CORE_H

#include <stddef.h>

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

typedef struct _shell_s
{
	int bValid;

	int bLogin;
	char *user_name;
	char *group_name;

	char workdir[MAX_PATH];

	#define BUF0_SIZE 256
	#define BUF1_SIZE 1024
	char buf_0[BUF0_SIZE];
	char *buf_1/*[BUF1_SIZE]*/;
	char *p_read;
	int read_status_code;
	//@value:
	//	( 0) -> `default`/`unknown error`
	//	(-1) -> `EOF`
	//	( 1) -> `normal`

	//! etc.
} shell_t;

//#- shellio.c -
int qsh_open(shell_t *psh);
void qsh_close(shell_t *psh);

#endif