// ****************************
// *
// * Auto-generated don't edit
// *
// ****************************
#ifndef SERVICETERMINATECAUSENUMSTOSTRING_H
#define SERVICETERMINATECAUSENUMSTOSTRING_H

#include "ServiceTerminateCause.h"

namespace SkyHobbit
{
namespace Common
{
namespace ServiceControl
{
class ServiceTerminateCauseEnumsToString {
public:
	static QString enumToString( ServiceTerminateCause::Enum val)
	{
		switch(val) {
			case ServiceTerminateCause::NotRunned: return "ServiceTerminateCause::NotRunned"; // 0
			case ServiceTerminateCause::ByCommand: return "ServiceTerminateCause::ByCommand"; // 1
			case ServiceTerminateCause::ByTerminate: return "ServiceTerminateCause::ByTerminate"; // 2
			case ServiceTerminateCause::ByKill: return "ServiceTerminateCause::ByKill"; // 3
		}
		return QString("undefined (%1)").arg(val);
	} // end of Enum
};
} // namespace Pw
} // namespace Common
} // namespace ServiceControl

#endif
