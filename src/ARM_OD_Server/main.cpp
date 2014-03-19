#include <QtCore/QCoreApplication>
#include <QObject>
#include <QMetaType>
#include <PwLogger/PwLogger.h>

#include "ARM_OD_Application.h"

#include "ARM_OD_Srv.h"

//#define PWLOGGERINIT_AGENTCONFNAME ("ARM_OD_view.log4qt")



int main(int argc, char *argv[])
{
//    Pw::Logger::PwLoggerFactory::Instance()->Configure(PWLOGGERINIT_AGENTCONFNAME);

    ARM_OD_Application a(argc, argv);

	Logger::setupLogger();

//    qRegisterMetaType<rpc_msg> ("rpc_msg");
//    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
//    qRegisterMetaType<quint32>("quint32");

    ARM_OD_Srv arm;
//    arm.start();



    return a.exec();
}
