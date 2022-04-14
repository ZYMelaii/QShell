#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <windows.h>

#include <qsh/uiman.h>
#include <qsh/qshw.h>

static int _qshg_counter = 0;
static void *_qshg_ptr = NULL;
static int _qshg_ui_status = 0;
static int _qshg_ui_mode = 0;

void qshuis_disable_ctrl_c()
{
	signal(SIGINT, SIG_IGN); //! ignore ctrl+c
}

void qshuis_enable_ctrl_c()
{
	signal(SIGINT, SIG_DFL); //! restore ctrl+c
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
		case CTRL_BREAK_EVENT: //! ctrl+break signal
		case CTRL_CLOSE_EVENT: //! close console
		case CTRL_LOGOFF_EVENT: //! user logout
		case CTRL_SHUTDOWN_EVENT: //! system shutdown
		{
			qshui_exit(0);
			qshw_xprint("QShell: interrupted and safely quited.\n");
			return TRUE;
		}
		case CTRL_C_EVENT: // ctrl+c signal
		{
			return TRUE;
		}
	}
	return FALSE;
}

void qshui_setup(int mode)
{
	if (mode != QSH_RAW && mode != QSH_CUI && mode != QSH_GUI)
	{
		qshw_xprint("QShell: \x02\031[WARNING] \x02\030unknown ui mode [%d]\n", mode);
		return;
	}
	
	if (_qshg_ui_status == 1)
	{
		qshw_xprint("QShell: \x02\031[WARNING] \x02\030ui envriment has setted\n");
		return;
	}

	//! init global vars
	_qshg_ptr = NULL;
	_qshg_ui_status = 1;

	switch (mode)
	{
		case QSH_RAW:
		{
			break;
		}
		case QSH_CUI:
		{
			//! disable ctrl-c signal
			qshuis_disable_ctrl_c();
			//! config handler
			if (SetConsoleCtrlHandler(qshuis_con_handler, TRUE) == 0)
			{
				qshw_xprint("QShell: \x02\031[ERROR] \x02\030failure@SetConsoleCtrlHandler\n");
				qshw_xprint("QShell: safely quited.\n");
				qshui_exit(-QSH_CUI);
			}
			break;
		}
		case QSH_GUI:
		{
			qshw_xprint("QShell: \x02\033[WARNING] \x02\030forbidden@qshui_setup(QSH_GUI)\n");
			qshw_xprint("QShell: safely quited.\n");
			qshui_exit(-QSH_GUI);
			break;
		}
	}
}

void qshui_cleanup()
{
	if (_qshg_ui_status == 1)
	{
		if (_qshg_ptr != NULL && _qshg_counter > 0)
		{
			qsh_close((shell_t*)_qshg_ptr);
			_qshg_ptr = NULL;
			--_qshg_counter;
		}
		_qshg_ui_status = 0;
	}
}

void qshui_exit(int exitCode)
{
	//! cleanup
	qshui_cleanup();

	if (_qshg_counter != 0)
	{
		qshw_xprint("QShell: [WARNING] unexcepted-exception@qshui_exit\n");
	}

	//! restore
	switch (_qshg_ui_mode)
	{
		case QSH_RAW:
		{
			break;
		}
		case QSH_CUI:
		{
			qshuis_enable_ctrl_c();
			SetConsoleCtrlHandler(NULL, FALSE);
			break;
		}
		case QSH_GUI:
		{
			break;
		}
	}

	//! exit
	exit(exitCode);
}

void qshui_set_global_ptr(void *ptr)
{
	_qshg_ptr = ptr;
	++_qshg_counter;
}