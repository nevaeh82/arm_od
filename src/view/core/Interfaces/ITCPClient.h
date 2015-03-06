#ifndef ITCPCLIENT_H
#define ITCPCLIENT_H

#include <QObject>
#include <QByteArray>

class ITCPClientOld: public QObject
{
public:
    virtual ~ITCPClientOld(){};

    virtual void set(QByteArray data)       = 0;
    virtual QByteArray get()                = 0;
    virtual bool get_state()                = 0;

public slots:
    virtual void slotDestroy()              = 0;

signals:
    void signalPing(bool);
};

#endif // ITCPCLIENT_H
