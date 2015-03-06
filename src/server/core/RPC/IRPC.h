#ifndef IRPC_H
#define IRPC_H

//#include <QtGlobal>

class IClient;
class QString;
class QByteArray;

class IRPC
{
public:
	virtual ~IRPC(){}
	virtual void sendDataByRpc(const QString& signalType, const QByteArray& data) = 0;

  /*  virtual int start()                                 = 0;
	virtual int stop()                                  = 0;*/
	virtual quint64 get_client_id(IClient* client)      = 0;
};

#endif // IRPC_H
