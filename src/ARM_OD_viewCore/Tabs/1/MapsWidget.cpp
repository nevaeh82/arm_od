#include "MapsWidget.h"

MapsWidget::MapsWidget()
{
    map = new QMap<QString, GraphicWidget *>;
}

MapsWidget::~MapsWidget()
{
    delete map;
}

void MapsWidget::setValue(QString name, GraphicWidget *gr)
{
    map->insert(name, gr);
}

QMap<QString, GraphicWidget *> *MapsWidget::getMap()
{
    return map;
}
