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

	return UavModel::getExtendedId( info );
}

QString UavModel::getExtendedId(const UavInfo& uav)
{
	uint source = uav.source;

	if ( source == UAV_SLICES_SOURCE ) {
		source = UAV_AUTOPILOT_SOURCE;
	}

	QString id = QString("%1_%2_%3")
			.arg(uav.uavId)
			.arg(source)
			.arg(uav.historical ? 1 : 0);

	return id;
}
