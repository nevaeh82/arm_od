#ifndef UAVMODEL_H
#define UAVMODEL_H

#include <QString>

#include "Tabs/DbBla/Defines.h"

class UavModel
{
public:
	UavModel();

	static QString getExtendedId(const Uav& uav, uint source);
	static QString getExtendedId(const UavInfo& uav);
};

#endif // UAV_H
