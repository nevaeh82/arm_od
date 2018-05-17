#ifndef ISERVICEHANDLER_H
#define ISERVICEHANDLER_H
#include <QString>
#include <QObject>
#include "ServiceTerminateCause.h"
#include "ProcessState.h"
#include "CommonCodes.h"

namespace SkyHobbit
{
namespace Common
{
namespace ServiceControl
{


class IServiceHandler
{

public:
	virtual ~IServiceHandler(){}

	/// @brief Gets server path.
	///
	virtual QString getServicePath() = 0;

	/// @brief Indicates whether server available
	///
	virtual bool isServiceAvailable() = 0;

	/// @brief Starts service
	///
	virtual void start(const bool, const bool) = 0;

public slots:

	/// @brief Terminate server.
	///
	virtual int terminate() = 0;

	/** Method returns opened map ID. (aka Process ID)

	  @return  process ID which provides map.
	  */
	virtual int serviceHandle() = 0;

	/** Indicates whether server started .
	  @return  true is server started.
	  */
	virtual bool isServiceStarted() = 0;


	/** Gets server filename without path;
	  */
	virtual QString serviceName() = 0;

	/// @brief Wait server started.
	/// @return true if server started successfully
	/// @param timeout   timeout in seconds.
	virtual bool waitService(int timeout) = 0;


	/**
	 * @brief Get server result code.
	 * @return result code.
	 */
	virtual ProcessState::Enum  getState() = 0;

	/**
	 * @brief Get server last error.
	 * @return error code.
	 */
	virtual SkyHobbit::Common::ServiceControl::ServiceErrorCodes::Enum getLastError() = 0;

};

} // namespace ServiceControl
} // namespace Common
} // namespace Pw

#endif // ISERVICEHANDLER_H
