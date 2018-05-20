// ****************************
// *
// * Auto-generated don't edit
// *
// ****************************
#ifndef SERVICEERRORNUMSTOSTRING_H
#define SERVICEERRORNUMSTOSTRING_H

#include "CommonCodes.h"

namespace SkyHobbit
{
namespace Common
{
namespace ServiceControl
{
class ServiceErrorEnumsToString {
public:
	static QString enumToString( ServiceErrorCodes::Enum val)
	{
		switch(val) {
			case ServiceErrorCodes::Success: return "ServiceErrorCodes::Success"; // 0
		}
		return QString("undefined (%1)").arg(val);
	} // end of Enum
};
} // namespace Pw
} // namespace Common
} // namespace ServiceControl

#endif
