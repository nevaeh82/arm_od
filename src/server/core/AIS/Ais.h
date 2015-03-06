#ifndef AIS_H
#define AIS_H

#include <QObject>
#include <QTimer>
#include "parser.h"
#include "../Common/IRouter.h"

class Ais : public QObject
{
    Q_OBJECT
public:
    Ais(IRouter* router);
    ~Ais();

private:
    Parser*         _parser;
    IRouter*        _router;
    QTimer*         _update_timer;

public slots:
    void    stop();

private slots:
    void _slot_update();

signals:
    void finished();

};

#endif // AIS_H
