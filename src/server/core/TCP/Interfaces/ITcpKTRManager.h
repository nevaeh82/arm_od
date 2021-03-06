#ifndef ITCPKTRMANAGER_H
#define ITCPKTRMANAGER_H

#include <QtGlobal>

class ITcpKTRManager
{
public:
	virtual ~ITcpKTRManager(){}
	virtual void connectToBoard(const QString& hostPort, const quint16& board, const quint32& device)	= 0;
	virtual void needToUpdateAfterDisconnect(const bool value, const QString& hostPost)	= 0;
};

#endif // ITCPKTRMANAGER_H
