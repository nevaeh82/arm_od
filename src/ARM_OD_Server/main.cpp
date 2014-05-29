#include <QtCore/QCoreApplication>

#include "ARM_OD_Application.h"
#include "ARM_OD_Srv.h"

int main(int argc, char *argv[])
{
	ARM_OD_Application a(argc, argv);

	if( a.isRunning() ) {
		return 0;
	}

	Logger::setupLogger("./logs/Server.log");

	ARM_OD_Srv arm;

	return a.exec();
}
