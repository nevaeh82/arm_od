#ifndef ITCPKTRMANAGER_H
#define ITCPKTRMANAGER_H

#include <QtGlobal>

class ITcpKTRManager
{
public:
	virtual ~ITcpKTRManager(){}
	virtual void connectToBoard(const QString& hostPort, const quint16& board, const quint32& device)	= 0;
};

#endif // ITCPKTRMANAGER_H
