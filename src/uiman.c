#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <windows.h>

#include "uiman.h"
#include "qshw.h"

static int _qsh_global_counter = 0;
static void *_qsh_global_ptr = NULL;
static int _qsh_global_ui_status = 0;

void qshuis_disable_ctrl_c()
{
	signal(SIGINT, SIG_IGN); // ignore ctrl+c
}

void qshuis_disable_con_close()
{
	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
	DrawMenuBar(hwnd);
}

void qshuis_enable_con_close()
{
	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);
	DrawMenuBar(hwnd);
}

BOOL WINAPI qshuis_con_handler(DWORD event)
{
	switch (event)
	{
		case CTRL_BREAK_EVENT: // ctrl+break signal
		case CTRL_CLOSE_EVENT: // close console
		case CTRL_LOGOFF_EVENT: // user logout
		case CTRL_SHUTDOWN_EVENT: // system shutdown
		{
			qshui_exit();
			printf("QShell: interrupted & safely quited.\n");
			return TRUE;
		}
		case CTRL_C_EVENT: // ctrl+c signal
		{
			return TRUE;
		}
	}
	return FALSE;
}

void qshui_setup()
{
	//@init global vars
	_qsh_global_ptr = NULL;
	_qsh_global_ui_status = 1;

	//@config winform
	qshuis_disable_ctrl_c();
	// qshuis_disable_con_close();

	//@config handler
	if (SetConsoleCtrlHandler(qshuis_con_handler, TRUE) == 0)
	{
		qshw_print(QSHW_RED, "[ERROR] failure@SetConsoleCtrlHandler\n");
	}
}

void qshui_cleanup()
{
	if (_qsh_global_ui_status == 1)
	{
		if (_qsh_global_ptr != NULL && _qsh_global_counter > 0)
		{
			qsh_close((shell_t*)_qsh_global_ptr);
			_qsh_global_ptr = NULL;
			--_qsh_global_counter;
		}
		_qsh_global_ui_status = 0;
	}
}

void qshui_exit()
{
	//@cleanup
	qshui_cleanup();
	if (_qsh_global_counter != 0)
	{
		// printf("[WARNING] unexcepted exception");
	}

	//@restore
	// qshuis_enable_con_close();
	SetConsoleCtrlHandler(NULL, FALSE);

	//@exit
	exit(0);
}

void qshui_set_global_ptr(void *ptr)
{
	_qsh_global_ptr = ptr;
	++_qsh_global_counter;
}