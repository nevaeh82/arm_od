#include "Ais.h"

Ais::Ais(IRouter *router)
{
    _router = router;
    _parser = new Parser(_router);

    _update_timer = new QTimer();

    connect(_update_timer, SIGNAL(timeout()), this, SLOT(_slot_update()));

	_parser->NewSquere(60.7209, 28.0289, 58.8187, 34.0933);//43.541194, 40.42694, 41.634183, 42.912167);

    _update_timer->start(20000);

}

Ais::~Ais()
{
    emit finished();
    delete _parser;
}

void Ais::stop()
{
    _update_timer->stop();
    delete this;
}

void Ais::_slot_update()
{
    _parser->ParserWork();
}
