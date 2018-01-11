#ifndef NIIPPCONTROLWIDGET_H
#define NIIPPCONTROLWIDGET_H

#include <QWidget>
#include "Niipp.h"
#include "onepointwgt.h"

namespace Ui {
class NiippWidget;
}

class NiippWidget : public QWidget
{
	Q_OBJECT

public:
	explicit NiippWidget(QWidget *parent = 0);
	~NiippWidget();

    void setMap(MapController* map);

private:
	Ui::NiippWidget *ui;
    int m_cnt;
    QMap<int, onepointwgt*> m_stationMap;
    MapController* m_map;
private slots:
    void onAddStation(double, double);
    void onAddStation(NStation cfg, int id);
    void stationRemove(int);
    void setMouse(bool);
    void loadStations();
};

#endif // NIIPPCONTROLWIDGET_H
