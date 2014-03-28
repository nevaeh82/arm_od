#ifndef TABSPROPERTY_H
#define TABSPROPERTY_H

#include <QString>
#include <QMap>

//#include "MainTab.h"

/*class TabsProperty
{
public:
    TabsProperty();

private:
    /// punkt Id
    int                 _id;
    /// name
    QString             _name;
    /// latitude
    double              _latitude;
    /// longitude
    double              _longitude;

public:
    void                destroy();
    int                 get_id();
    QString             get_name();
    double              get_latitude();
    double              get_longitude();


    void set_id(int id);
    void set_name(QString name);
    void set_latitude(double lat);
    void set_longitude(double lon);
};*/

typedef struct Station{
	int id;
	QString name;
	double latitude;
	double longitude;
}Station;

#endif // TABSPROPERTY_H
