// ****************************
// *
// * Auto-generated don't edit
// *
// ****************************
#ifndef PROCESSSTATENUMSTOSTRING_H
#define PROCESSSTATENUMSTOSTRING_H

#include "ProcessState.h"

namespace SkyHobbit
{
namespace Common
{
namespace ServiceControl
{
class ProcessStateEnumsToString {
public:
	static QString enumToString( ProcessState::Enum val)
	{
		switch(val) {
			case ProcessState::NotRunning: return "ProcessState::NotRunning"; // 0
			case ProcessState::IsRunnung: return "ProcessState::IsRunnung"; // 1
			case ProcessState::Error: return "ProcessState::Error"; // 2
		}
		return QString("undefined (%1)").arg(val);
	} // end of Enum
};
} // namespace Pw
} // namespace Common
} // namespace ServiceControl

#endif
