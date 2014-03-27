#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

#include "PrmSimulator.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if (a.arguments().length() < 2) {
		qCritical() << "Expected port number in first argument. By default use 24500.";
		return 1;
	}

	// check port in arguments
	bool intOK;
	int port = a.arguments().at(1).toInt( &intOK );

	if (!intOK) {
		qCritical() << "Wrong port format";
		return 2;
	}

	// create simulator object
	PrmSimulator simulator( port );
	if (!simulator.start()) {
		qCritical() << "Can't start simulator";
		return 3;
	}

	return a.exec();
}
