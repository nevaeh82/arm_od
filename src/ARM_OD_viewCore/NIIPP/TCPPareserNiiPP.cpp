#include "TCPPareserNiiPP.h"
#include <QDebug>

TCPPareserNiiPP::TCPPareserNiiPP()/*:
	_type(0),*/
{
}

TCPPareserNiiPP::~TCPPareserNiiPP()
{
	qDebug() << "Terminated parser";
}

/// self distroy
void TCPPareserNiiPP::distruct()
{
	delete this;
}

void TCPPareserNiiPP::set_router(IRouter *router)
{
	Q_UNUSED( router );
//    _queue = router->get_queue();
}

void TCPPareserNiiPP::parsing(QByteArray data)
{

}
