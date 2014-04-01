#ifndef MAPSWIDGET_H
#define MAPSWIDGET_H

#include "../Graphics/GraphicWidget.h"

class MapsWidget
{
public:
    MapsWidget();
    ~MapsWidget();

public:
    void setValue(QString, GraphicWidget *);
    QMap<QString, GraphicWidget *> *getMap();

private:
    QMap<QString, GraphicWidget *> *map;
};

#endif // MAPSWIDGET_H
