#include <QApplication>
#include "mainwindow.h"
#include <QNetworkDiskCache>
#include <QDir>
#include <stdlib.h>
#include <stdio.h>
#include <QThread>
#include <QMutexLocker>
#include <QMutex>
#include <QStringList>
#include "Options.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setOrganizationName("nppntt.ru");
	a.setApplicationName("GisClient");


	QString str;
	for(int i = 0; i < a.arguments().count(); i++) {
		str = a.arguments().at(i);
		if(str.startsWith("--testJs=")) {
			Options::testJs = str.split("=").at(1).toLower() == "yes";
		}
	}

	MainWindow w;
	w.show();

	return a.exec();
}
