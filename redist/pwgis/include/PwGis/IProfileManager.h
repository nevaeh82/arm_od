#ifndef IPROFILEMANAGER_H
#define IPROFILEMANAGER_H
#include <QString>

namespace Pw { namespace Gis {

/**
 * @brief The ProfileManager class provides service for installing (registering)
 *  and uninstalling (unregistering) profiles from server profiles filesystem.
 */
class IProfileManager
{
public:
	virtual ~IProfileManager(){}

	/**
	 * @brief Register profile in <Profiles> subsystem.
	 * @param profileName  name of profile.
	 * @param profileDir   source pfile directory.
	 * @returns false on error true otherwise.
	 */
	virtual bool registerProfile(const QString& profileName, const QString& profileDir )=0;


	/**
	 * @brief Unregister profile from profiles filesystem.
	 * @param profileName name of profile.
	 */
	virtual void unregisterProfile(const QString& profileName)=0;
};

}}


#endif // IPROFILEMANAGER_H
