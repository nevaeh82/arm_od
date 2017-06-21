#include <QtCore/QCoreApplication>

#include "ARM_OD_Application.h"
#include "ARM_OD_Srv.h"

#include <Windows.h>
#include <Dbghelp.h>
#include <stdio.h>

#include <Logger/Logger.h>

typedef QVector<QPointF>         rpc_send_points_vector;

HWND hwnd;
static char * g_output = NULL;
static LPTOP_LEVEL_EXCEPTION_FILTER g_prev = NULL;

LONG WINAPI exception_filter(LPEXCEPTION_POINTERS info)
{
	return EXCEPTION_EXECUTE_HANDLER;
}

static void
backtrace_register(void)
{
	LoadLibraryA("msvcrt.dll");

	DWORD dwMode = SetErrorMode(SEM_NOGPFAULTERRORBOX);
	SetErrorMode(dwMode | SEM_NOGPFAULTERRORBOX);

	if (g_output == NULL) {
		g_output = (char*)malloc(sizeof(long));
		g_prev = SetUnhandledExceptionFilter(exception_filter);
	}
}



int main(int argc, char *argv[])
{
	ARM_OD_Application a(argc, argv);

	if( a.isRunning() ) {
		return 0;
	}

	Logger::setupLogger("./logs/Server.log");

	hwnd = ::GetDesktopWindow();
	backtrace_register();

	ARM_OD_Srv arm;

	return a.exec();
}
