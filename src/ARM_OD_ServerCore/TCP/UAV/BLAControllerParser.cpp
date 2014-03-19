#include "BLAControllerParser.h"
#include <QDebug>

BLAControllerParser::BLAControllerParser(IBLAController *controller)
{
    _controller = controller;
}

BLAControllerParser::~BLAControllerParser()
{
}

void BLAControllerParser::set_router(IRouter *router)
{
    _router = router;
}

void BLAControllerParser::parsing(QByteArray data)
{
    qDebug() << "parse data from KTR" << QString::fromAscii(data.constData(), data.size());
	if (data.mid(0, 12) != "control link") {
		qDebug() << "VSE PLOHO";
	}
    data.remove(0, 12);
    int count_bla = data.at(0);
    QVector<quint16> ids;
    for(int i = 0; i < count_bla; i++)
    {
        quint16 id = ((quint8)data.at(3 + i*24) << 8) + (quint8)data.at(2 + i*24);
        ids.push_back(id);
        qDebug() << id;
    }
    _controller->connect_bla(ids);
}

void BLAControllerParser::distruct()
{
    delete this;
}
