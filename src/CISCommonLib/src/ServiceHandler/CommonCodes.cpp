#include <QObject>
#include "CommonCodes.h"

namespace SkyHobbit
{
namespace Common
{
namespace ServiceControl
{
QString ServiceErrorCodes::getErrorString( Enum value )
{
	static QMap< Enum, QString > codeDescriptions;

	if ( codeDescriptions.size() == 0 ) {
		//lazy init

		codeDescriptions[ Success ] = "Success.";
	}

	return codeDescriptions.value( value );
}
} // namespace ServiceControl
} // namespace Common
} // namespace Pw
