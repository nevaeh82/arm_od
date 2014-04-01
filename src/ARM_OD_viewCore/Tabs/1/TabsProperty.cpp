#include "TabsProperty.h"

TabsProperty::TabsProperty()
{
    _grafic_widget = NULL;
}

void TabsProperty::destroy()
{
    delete this;
}

void TabsProperty::set_id(int id)
{
    _id = id;
}

void TabsProperty::set_name(QString name)
{
    _name = name;
}

void TabsProperty::set_ip(QString ip)
{
    _ip = ip;
}

void TabsProperty::set_port(int port)
{
    _port = port;
}

void TabsProperty::set_ip_Flakon(QString ip)
{
    _ip_Flakon = ip;
}

void TabsProperty::set_port_Flakon(int port)
{
    _port_Flakon = port;
}

void TabsProperty::set_graphic_widget(GraphicWidget *gr)
{
    _grafic_widget = gr;
}

void TabsProperty::set_latitude(double lat)
{
    _latitude = lat;
}

void TabsProperty::set_longitude(double lon)
{
    _longitude = lon;
}

int TabsProperty::get_id()
{
    return _id;
}

QString TabsProperty::get_name()
{
    return _name;
}

QString TabsProperty::get_ip()
{
    return _ip;
}

int TabsProperty::get_port()
{
    return _port;
}

QString TabsProperty::get_ip_Flakon()
{
    return _ip_Flakon;
}

int TabsProperty::get_port_Flakon()
{
    return _port_Flakon;
}

double TabsProperty::get_latitude()
{
    return _latitude;
}

double TabsProperty::get_longitude()
{
    return _longitude;
}

GraphicWidget *TabsProperty::get_graphic_widget()
{
    return _grafic_widget;
}

void TabsProperty::set_graphic_widget_correlation(int id, GraphicWidget *gr_corr)
{
    _map_correlations.insert(id, gr_corr);
}

QMap<int, GraphicWidget *> &TabsProperty::get_correlations()
{
    return _map_correlations;
}
