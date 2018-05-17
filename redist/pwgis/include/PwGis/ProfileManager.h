#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H
#include <QObject>
#include <QTextStream>
#include <IProfileManager.h>
#include "PwGisClient.h"

namespace Pw { namespace Gis { namespace Common {

class IServerEnvironment;
}}}

namespace Pw { namespace Gis {

using namespace Pw::Gis::Common;

/**
 * @brief The ProfileManager class provides service for installing (registering)
 *  and uninstalling (unregistering) profiles from server profiles filesystem.
 */
class GISCLIENT_EXPORT ProfileManager : public QObject, public IProfileManager
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{2fb4ae7f-46c9-411c-936d-7d1259144efa}")
	Q_CLASSINFO("InterfaceID", "{92f7a447-0bd7-4e71-b8b0-73ed6f36b7e7}")
	Q_CLASSINFO("CoClassAlias", "ProfileManager")

private:
	IServerEnvironment* _env;

private:

	/**
	 * @brief Get and create if need directory for profile.
	 * @param profileName  profile name
	 * @return  full directory name for profile.
	 */
	QString getProfileDir(QString profileName );

public:
	/**
	 * @brief Creates a new instance of ProfileManager class with default enviroment.
	 * @param parent  parent QObject.
	 */
	ProfileManager(QObject* parent=0);

	/**
	 * @brief Creates a new instance of ProfileManager class.
	 * @param env  Server environment.
	 * @param parent  parent QObject.
	 */
	ProfileManager(IServerEnvironment* env, QObject* parent);


	virtual ~ProfileManager();

public slots:
	/**
	 * @brief Register profile in <Profiles> subsystem.
	 * @param profileName  name of profile.
	 * @param profileDir   source pfile directory.
	 * @returns false on error true otherwise.
	 */
	virtual bool registerProfile(const QString& profileName, const QString& profileDir );


	/**
	 * @brief Unregister profile from profiles filesystem.
	 * @param profileName name of profile.
	 */
	virtual void unregisterProfile(const QString& profileName);

};

}}
#endif // PROFILEMANAGER_H
