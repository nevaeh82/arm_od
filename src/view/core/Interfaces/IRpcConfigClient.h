#ifndef IRPCCONFIGCLIENT_H
#define IRPCCONFIGCLIENT_H

class QString;

class IRpcConfigClient
{

public:
	virtual ~IRpcConfigClient(){}

	virtual bool isConnected() = 0;
	virtual void requestGetStationList(const QString& filename)			= 0;
	virtual void requestGetMapObjects(const QString& filename)	= 0;
	virtual void requestGetMapPoints(const QString& filename)	= 0;
	virtual void requestGetDbConfiguration(const QString& filename)		= 0;
};

#endif // IRPCCONFIGCLIENT_H
