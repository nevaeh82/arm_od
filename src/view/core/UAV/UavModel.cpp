#include <QDebug>

#include "UavModel.h"

UavModel::UavModel()
{
}

QString UavModel::getExtendedId(const Uav& uav, uint source)
{
	UavInfo info;
	info.uavId = uav.uavId;
	info.historical = uav.historical;
	info.source = source;
    info.name = uav.name;

//	if(info.historical) {
//		return UavModel::getExtendedIdHist( info );
//	}

	return UavModel::getExtendedId( info );
}

QString UavModel::getExtendedId(const UavInfo& uav)
{
//	uint source = uav.source;

//	if ( source == UAV_SLICES_SOURCE ) {
//		source = UAV_AUTOPILOT_SOURCE;
//	}

//	QString id = QString("%1_%2_%3")
//			.arg(uav.uavId)
//			.arg(source)
//			.arg(uav.historical ? 1 : 0);

    QString id = uav.name;

	if(uav.source == 100) {
		id = id+"-"+QObject::tr("A");
	} else if(uav.source == 101) {
		id = id+"-"+QObject::tr("M");
	}

	return id;
}

QString UavModel::getExtendedIdHist(const UavInfo& uav)
{
//	uint source = uav.source;

//	if ( source == UAV_SLICES_SOURCE ) {
//		source = UAV_AUTOPILOT_SOURCE;
//	}

//	QString id = QString("%1_%2_%3")
//			.arg(uav.uavId)
//			.arg(source)
//			.arg(uav.historical ? 1 : 0);

    QString id = uav.name;
    id = QObject::tr( "%1-H" ).arg( id );

    return id;
}
