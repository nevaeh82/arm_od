#include <QtGui/QApplication>
#include <QTranslator>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QGraphicsObject>
#include <QMetaType>

#include "MainWindow.h"
#include "MainWindowController.h"

#include <Logger/Logger.h>

typedef QVector<QPointF>         rpc_send_points_vector;

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	Logger::setupLogger("./logs/ArmOD.log");

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
