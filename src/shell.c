#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <windows.h>

void BetterPrint(int color_id, const char *format, ...);

//#- core functions declaration-
int qsh_open(shell_t *psh);
void qsh_reset_cmdline_ptr(shell_t *psh);
void qsh_close(shell_t *psh);
void qsh_readline(shell_t *psh);
const char *qsh_get_cmdline(shell_t *psh);
//#- ENDLINE -

void PrintPrompt(shell_t *psh);

int qsh_open(shell_t *psh)
{
	if (psh == NULL) return -1;

	memset(psh, 0, sizeof(shell_t));

	psh->user_name = strdup("ZYmelaii");
	psh->group_name = strdup("admin");
	psh->bLogin = 1;

	char buf[MAX_PATH];
	getcwd(buf, sizeof(buf));
	char drive;
	sscanf(buf, "%c:%[^\n]", &drive, psh->workdir + 2);
	char *p = psh->workdir;
	*p++ = '/';
	*p++ = drive + 32;
	do {
		if (*p == '\\')
		{
			*p = '/';
		} else if (*p >= 'A' && *p <= 'Z')
		{
			*p += 32;
		}
	} while (*++p != '\0');

	psh->buf_1 = (char*)malloc(BUF1_SIZE + 1);
	memset(psh->buf_1, 0, BUF1_SIZE + 1);

	psh->bValid = 1;
	return 0;
}

void qsh_close(shell_t *psh)
{
	if (psh == NULL) return;

	if (psh->bLogin == 1)
	{
		free(psh->user_name);
		free(psh->group_name);
	}

	free(psh->workdir);

	qsh_reset_cmdline_ptr(psh);
	free(psh->buf_1);

	psh->bValid = 0;
}

#define qsh_set_signal signal
// void qsh_set_signal(int sig, void (*func)(int))
// {
// 	signal(sig, func);
// }

void qsh_reset_cmdline_ptr(shell_t *psh)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);

	psh->read_status_code = 0;
	if (psh->p_read == NULL) return;

	if (psh->p_read != psh->buf_0 && psh->p_read != psh->buf_1)
	{
		free(psh->p_read);
	}

	psh->p_read = NULL;
}

void qsh_readline(shell_t *psh)
{	// read input cmdline and save the result to psh->p_read
	//@warning: there're some problems facing ctrl-v behaviour
	assert(psh != NULL);
	assert(psh->bValid == 1);

	qsh_reset_cmdline_ptr(psh);

	char format[32];

	sprintf(format, "%%%d[^\n]", BUF0_SIZE);
	int bRet = scanf(format, psh->buf_0);
	// printf("&> bRet[0]=%d\n", bRet);

	if (bRet <= 0)
	{
		if (bRet == -1)
		{
			psh->read_status_code = -1;
		}
		fflush(stdin);
		return;
	}

	int nCount = strlen(psh->buf_0);
	if (nCount == BUF0_SIZE)
	{	// overflow - 1
		sprintf(format, "%%%d[^\n]", BUF1_SIZE);
		bRet = scanf(format, psh->buf_1);
		// printf("&> bRet[1]=%d\n", bRet);
		fflush(stdin);
		nCount = strlen(psh->buf_1);
		if (nCount == BUF1_SIZE)
		{	// overflow - 2, report an ERROR and exit
			BetterPrint(1, "[ERROR] An overflow occurred while reading cmdline.\n");
			return;
		} else if (nCount + BUF0_SIZE > BUF1_SIZE)
		{	// merge buffer
			psh->p_read = (char*)malloc(BUF0_SIZE + nCount + 1);
			strcat(psh->p_read, psh->buf_0);
			strcat(psh->p_read, psh->buf_1);
		} else if (bRet == 1) {
			char *p = psh->buf_1 + nCount + 1, *q = p + BUF0_SIZE;
			do { *--q = *--p; } while (p != psh->buf_1);
			memcpy(psh->buf_1, psh->buf_0, BUF0_SIZE);
			psh->p_read = psh->buf_1;
		}
	}

	if (psh->p_read == NULL)
	{
		fflush(stdin);
		psh->p_read = psh->buf_0;
	}

	psh->read_status_code = 1;
}

const char *qsh_get_cmdline(shell_t *psh)
{
	return psh->p_read;
}

void PrintPrompt(shell_t *psh)
{
	assert(psh != NULL);
	assert(psh->bValid == 1);

	BetterPrint(2, "%s@%s:", psh->user_name, psh->group_name);
	BetterPrint(3, psh->workdir);
	BetterPrint(0, "$ ");

	fflush(stdout);
}

int main(int argc, char const *argv[])
{
	qsh_set_signal(SIGINT, SIG_IGN); // ignore ctrl+c

	shell_t sh;
	qsh_open(&sh);

	while (1)
	{
		PrintPrompt(&sh);
		qsh_readline(&sh);
		const char *cmdline = qsh_get_cmdline(&sh);
		if (cmdline != NULL)
		{
			//#- Add eval.. procedure here -
			if (strcmp(cmdline, "exit") == 0)
			{
				break;
			} else if (strcmp(cmdline, "clear") == 0)
			{
				system("cls");
			} else
			{
				BetterPrint(0, "QShell: `%s` command not found.\n", cmdline);
			}
			//#- ENDLINE -
		}
		if (sh.read_status_code == -1)
		{
			printf("\n");
		}
	}

	qsh_close(&sh);
	return 0;
}

void BetterPrint(int color_id, const char *format, ...)
{
	// FOREGROUND_INTENSITY,FOREGROUND_RED,FOREGROUND_GREEN,FOREGROUND_BLUE
	// BACKGROUND_INTENSITY,BACKGROUND_RED,BACKGROUND_GREEN,BACKGROUND_BLUE

	WORD colorCode[] = {
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // white
		FOREGROUND_RED, // red
		FOREGROUND_GREEN, // green
		FOREGROUND_BLUE, // blue
		FOREGROUND_RED | FOREGROUND_GREEN,
		FOREGROUND_RED | FOREGROUND_BLUE,
		FOREGROUND_GREEN | FOREGROUND_BLUE,
	};
	const int colorNum = sizeof(colorCode)/sizeof(WORD);


	if (color_id < 0 || color_id >= colorNum)
	{
		color_id = 0;
	}

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, colorCode[color_id]);

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	SetConsoleTextAttribute(handle, colorCode[0]);
}
