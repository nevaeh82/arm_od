#ifndef COMMONCODES_H
#define COMMONCODES_H

#include <QString>
#include <QMap>

/** Server and client codes
  */
namespace SkyHobbit
{
namespace Common
{
namespace ServiceControl
{

class ServiceErrorCodes
{

public:
	enum Enum {
		Success = 0
	};


	static QString getErrorString( Enum value );
};
} // namespace ServiceControl
} // namespace Common
} // namespace Pw

#endif // COMMONCODES_H
