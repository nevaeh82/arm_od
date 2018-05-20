#ifndef ServiceTerminateCause_H
#define ServiceTerminateCause_H

namespace SkyHobbit
{
namespace Common
{
namespace ServiceControl
{

class ServiceTerminateCause
{
public:
	enum Enum {
		NotRunned = 0,
		ByCommand = 1,
		ByTerminate = 2,
		ByKill = 3
	};
};

} //ServerControl
} //Common
} //Pw

#endif // ServiceTerminateCause_H
