#ifndef TABSPROPERTY_H
#define TABSPROPERTY_H

#include <QString>
#include <QMap>

//#include "MainTab.h"

#include "../Graphics/GraphicWidget.h"

class TabsProperty
{
public:
    TabsProperty();

private:
    /// punkt Id
    int                 _id;
    /// name
    QString             _name;
    /// IP address
    QString             _ip;
    /// port
    int                 _port;
    /// IP address of Flakon
    QString             _ip_Flakon;
    /// port of Flakon
    int                 _port_Flakon;
    /// latitude
    double              _latitude;
    /// longitude
    double              _longitude;


    GraphicWidget       *_grafic_widget;

    QMap<int, GraphicWidget *>  _map_correlations;

public:
    void                destroy();
    int                 get_id();
    QString             get_name();
    QString             get_ip();
    int                 get_port();
    QString             get_ip_Flakon();
    int                 get_port_Flakon();
    GraphicWidget       *get_graphic_widget();
    double              get_latitude();
    double              get_longitude();
    QMap<int, GraphicWidget *> &get_correlations();


    void set_id(int id);
    void set_name(QString name);
    void set_ip(QString ip);
    void set_port(int port);
    void set_ip_Flakon(QString ip);
    void set_port_Flakon(int port);
    void set_latitude(double lat);
    void set_longitude(double lon);

    void set_graphic_widget(GraphicWidget *gr);
    void set_graphic_widget_correlation(int id, GraphicWidget *gr_corr);
};

#endif // TABSPROPERTY_H
