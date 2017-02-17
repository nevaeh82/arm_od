#include <QtGui/QApplication>
#include <QTranslator>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QGraphicsObject>
#include <QMetaType>

#include "MainWindow.h"
#include "MainWindowController.h"

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
     //SymCleanup(GetCurrentProcess());

//    MessageBoxA(hwnd,
//                QObject::tr("Error").toStdString().c_str(),
//                QObject::tr("Error").toStdString().c_str(),
//                MB_APPLMODAL);

    QProcess p;
    p.start("a.bat");
    p.waitForStarted(10000);

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

	QApplication a(argc, argv);
	Logger::setupLogger("./logs/ArmOD.log");

	hwnd = ::GetDesktopWindow();
	backtrace_register();

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	QCoreApplication::setOrganizationName("TRK");
	QCoreApplication::setApplicationName("Zaviruha");
	QApplication::setGraphicsSystem("opengl");

	Q_INIT_RESOURCE(translations);

	QTranslator translator;
	translator.load(":/ARM-OD_ru.qm");
	a.installTranslator(&translator);

	Q_INIT_RESOURCE(style);

	QFile file(":/qdarkstyle/style.qss");
	if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		a.setStyleSheet(file.readAll());
		file.close();
	}

	MainWindow view;

	MainWindowController controller;
	controller.appendView(&view);

	view.show();

	return a.exec();
}
