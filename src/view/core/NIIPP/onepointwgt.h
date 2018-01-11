#ifndef ONEPOINTWGT_H
#define ONEPOINTWGT_H

#include <QWidget>
#include "onepointwidgetadditional.h"
#include "nstation.h"
#include "Map/MapController.h"
#include "onepointwidgetadditional.h"

namespace Ui {
class onepointwgt;
}

class onepointwgt : public QWidget
{
    Q_OBJECT

public:
    explicit onepointwgt(int id, NStation cfg, MapController* map, QWidget *parent = 0);
    ~onepointwgt();

    NStation getCurrentConfig();
    void setCurrentConfig(NStation param);
 private:
    Ui::onepointwgt *ui;
    int m_id;
    MapController* m_map;
    onepointwidgetadditional* wgtadd;

    QPixmap m_pvoPixmap;
    QPixmap m_rebPixmap;

private slots:
    void gotoRemove();
    void stationChanged();

signals:
    void onRemove(int);
    void onStationChanged(NStation);
};

#endif // ONEPOINTWGT_H
