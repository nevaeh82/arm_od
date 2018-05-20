#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QString>

#include "IMapManager.h"
#include "MapManagerEvents.h"


class QWebView;
class PwGisWidget;
class QUrl;
class QMutex;

namespace Pw{ namespace Gis { namespace Common { namespace ServerControl {
class IServerRepository;
class IProxyServerHandlerFactory;
class IServerHandlerFactory;
class IServerHandler;
}}}}


using namespace Pw::Gis::Common::ServerControl;

class TestMapManager;
class TestMapOpen;
class TestInvalidMapOpen;

namespace Pw { namespace Gis { namespace Client {
class IGisControl;
}}}

namespace Pw { namespace Gis {

class MapManagerPrivate;

using namespace Pw::Gis::Client;

#define SERVER_INDEX_BASE ("/file/index.html")

class GISCLIENT_EXPORT MapManager : public MapManagerEvents, public IMapManager
{
	Q_OBJECT
	Q_CLASSINFO("ClassID","{1f6480ec-f341-4389-b651-7b31e4a24944}")
	Q_CLASSINFO("InterfaceID","{11b75101-c98a-4bd3-817e-4e39903f4212}")
	Q_CLASSINFO("EventsID","{bc6b88d6-f962-40a5-8eb0-39f319868e77}")
	Q_CLASSINFO("CoClassAlias", "MapManager")

	friend class ::TestMapManager;
	friend class ::TestInvalidMapOpen;
	friend class ::TestMapOpen;

	friend class MapManagerPrivate;

private:
	MapManagerPrivate* _data;

	IGisControl* _control;

	IServerRepository *_portManager;
	IServerHandler* _serverHandler;
	QString _serverIndex;
	Pw::Logger::ILogger* _logger;
	QString _serverPath;
	QString _atlasPath;
	QString _profileName;
	int _serverPort;
	bool _isOpeningMap;

	IServerHandlerFactory* _serverFactory;
	IProxyServerHandlerFactory* _atlasFactory;
	QMutex _syncRoot;
	QString _mapFilter;

	void replacePortManager(IServerRepository* newInstance );

private:
	void showProgress(PwGisWidget::Progress processStatus, bool isSuccess);
	void setWebViewContent(const QString& information, int progress);
	bool initMap(const QString& mapFile);
	void terminateServer();
	bool startServer(int timeout);

	bool openMapInCurrentServer(const QString& mapFile, int timeout);

	/**
	   @brief Initialization string-filter for openMapDialog
	  */
	void initMapFilter();

	/**
	   @brief Gets full name of the exe file
	   @return full name
	  */
	QString getExeName( QString name );

	/**
	   @brief Gets server supported map formats
	   @return map format collection
	  */
	MapFormatsCollection* getSupportedFormats();

public:
	MapManager(QObject* parent);
	MapManager(IGisControl* control, QObject* parent);
	~MapManager();

	virtual MapManagerEvents&  events() { return *this; }

	/**
	 *
	 * @brief Load test page.
	 * @param url
	 */
	void loadPage(QUrl &url);


public slots:
	/// Open map specified by map file and start map server if need.
	/// @param mapFile  map file.
	/// @param timeout  server staring timeout in seconds.
	/// @return true on success
	virtual bool openMap(QString mapFile, int timeout);

	/// Open map dialog and return selected map file name.
	/// @param mapDirectory  directory with maps for dialog
	/// @return  selected map file name
	virtual QString openMapDialog( QString mapDirectory );

	/// Load map from local started map server.
	/// @param server  map server name or IP address
	/// @param port  server port. usually used 8080
	virtual void openMapFrom(QString server, int port);

	/// Sends shutdown signal to map server
	virtual void closeMap();

	/// Starts Atlas server and opens map list.
	/// @param port  server port. usually used 8080, zero for autodetection
	/// @return true on success
	virtual bool openAtlas( int port = 0 );

	/// Strat Atlas server and opens map by it ID in servermaps or usermaps
	/// @param port  server port. usually used 8080, zero for autodetection
	/// @param id    unified map id for atlas
	virtual bool openAtlasMap( int port, const QString& mapId );

	/// Gets map objects factory
	/// @param server  map server name or IP address
	/// @param port    server port. usually used 8080, zero for autodetection
	/// @param mapId   unified map id for atlas
	virtual void openAtlasFrom( QString server, int port, const QString& mapId = QString() );

	/// Sends shutdown signal to atlas server
	virtual void closeAtlas();

	/// Set start page for server, for example "file/index.html".
	virtual void setIndexName( const QString& indexName );

	/// @brief Set server path if server palced is not in current directory.
	/// @param serverPath full server pathname, ex: c:\pwgis\PwGisServerApp.exe
	virtual void setServerPath( const QString& serverPath );

	/// @brief Set atlas path if atlas placed is not in current directory.
	/// @param atlasPath full server pathname, ex: c:\pwgis\PwGisAtlas.exe
	virtual void setAtlasPath( const QString& atlasPath );

	/// @brief Set profile name withc need to use with map.
	/// @param profileName  name of profile whtn must be registered in server.
	virtual void setProfile( const QString& profileName );

	/// @brief get last error code
	/// @return error code at server fault startup
	virtual int getErrorCode();

	/// @brief get port of running server
	virtual int getServerPort();

	virtual bool isOpeningMap();

protected slots:
	void loadWebViewStarted();
	void loadWebViewFinished( bool ok, const QUrl& url );
};

}}

#endif // MAPMANAGER_H
