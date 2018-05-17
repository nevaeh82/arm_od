#ifndef IMAPMANAGER_H
#define IMAPMANAGER_H
#include <QString>
#include "MapFormatsCollection.h"

class QWebView;

namespace Pw { namespace Gis {

class MapManagerEvents;

class IMapManager
{
public:
	/// Virtual destructor
	virtual ~IMapManager(){}

	/// Returns an event handler object
	/// @return QObject reference
	virtual MapManagerEvents& events() = 0;

	/// Open map specified by map file and start map server if need.
	/// @param mapFile  map file.
	/// @param timeout  server staring timeout in seconds.
	/// @return true on success
	virtual bool openMap(QString mapFile, int timeout=10) = 0;

	/// Open map dialog and return selected map file name.
	/// @param mapDirectory  directory with maps for dialog
	/// @return  selected map file name
	virtual QString openMapDialog( QString mapDirectory = "" ) = 0;

	/// Load map from local started map server.
	/// @param server  map server name or IP address
	/// @param port    server port. usually used 8080
	virtual void openMapFrom(QString server, int port) = 0;

	/// Sends shutdown signal to map server
	virtual void closeMap() = 0;

	/// Starts Atlas server and opens map list.
	/// @param port  server port. usually used 8080, zero for autodetection
	/// @return true on success
	virtual bool openAtlas( int port = 0 ) = 0;

	/// Strat Atlas server and opens map by it ID in servermaps or usermaps
	/// @param port  server port. usually used 8080, zero for autodetection
	/// @param id    unified map id for atlas
	virtual bool openAtlasMap( int port, const QString& mapId ) = 0;

	/// Gets map objects factory
	/// @param server  map server name or IP address
	/// @param port    server port. usually used 8080, zero for autodetection
	/// @param mapId   unified map id for atlas
	virtual void openAtlasFrom( QString server, int port, const QString& mapId = QString() ) = 0;

	/// Sends shutdown signal to atlas server
	virtual void closeAtlas() = 0;

	/// Set start page for server, for example "file/index.html".
	virtual void setIndexName( const QString& indexName ) = 0;

	/// @brief Set server path if server palced is not in current directory.
	/// @param serverPath full server pathname, ex: c:\pwgis\PwGisServerApp_gcc.exe
	virtual void setServerPath( const QString& serverPath) = 0;

	/// @brief Set atlas path if atlas placed is not in current directory.
	/// @param atlasPath full server pathname, ex: c:\pwgis\PwGisAtlas.exe
	virtual void setAtlasPath( const QString& atlasPath) = 0;

	/// @brief Set profile name withc need to use with map.
	/// @param profileName  name of profile whtn must be registered in server.
	virtual void setProfile( const QString& profileName ) = 0;

	/// @brief get last error code
	/// @return error code at server fault startup
	virtual int getErrorCode() = 0;

	/// @brief get port of running server
	virtual int getServerPort() = 0;

	///@brief is opening map state
	virtual bool isOpeningMap() = 0;
};

} // namespace Gis
} // namespace Pw

#endif // IMAPMANAGER_H
