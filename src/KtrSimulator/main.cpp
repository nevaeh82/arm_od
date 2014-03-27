#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

#include "KtrSimulator.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if (a.arguments().length() < 2) {
		qCritical() << "Expected port number in first argument. By default use 64300.";
		return 1;
	}

	// check port in arguments
	bool intOK;
	int port = a.arguments().at(1).toInt( &intOK );

	if (!intOK) {
		qCritical() << "Wrong port format";
		return 2;
	}

	// check if count of BPLA specified in arguments
	int bplaCount = 5;
	if (a.arguments().length() > 2) {
		int number = a.arguments().at(2).toInt( &intOK );

		if (intOK) bplaCount = number;
	}

	// create simulator object
	KtrSimulator simulator( port, bplaCount );
	if (!simulator.start()) {
		qCritical() << "Can't start simulator";
		return 3;
	}

	return a.exec();
}
