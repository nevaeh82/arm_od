#ifndef ITCPRECEIVER_H
#define ITCPRECEIVER_H

class QVariant;
class ITcpReceiver
{

public:
	virtual ~ITcpReceiver() {}
	virtual void onDataReceived(const QVariant& argument)	= 0;
};

#endif // ITCPRECEIVER_H
