#ifndef DEMOOBJECTSTIMER_H
#define DEMOOBJECTSTIMER_H

#include <QMainWindow>
#include <QFileDialog>

#include <PwGis/pwgiswidget.h>

namespace Ui {
class DemoObjectsTimer;
}

namespace Pw { namespace Gis {
class IMapBounds;
class IMapManager;
}}


class DemoObjectsTimer : public QMainWindow
{
    Q_OBJECT

public:
    explicit DemoObjectsTimer(QWidget *parent = 0);
    ~DemoObjectsTimer();

private:
    Ui::DemoObjectsTimer *ui;
    PwGisWidget *pwgiswidget;
    Pw::Gis::IMapManager *_mapManager;
    Pw::Gis::IMapBounds *_mapBounds;

    QTime *currentTimeStart;
    QStringList objNums;
    QString logString;
    bool startBenchmarkFlag;

    void startTimeCount();
    void getTimeResult(QString message);

    void addPolygons(bool useTransaction = false, int num = 0);
    void addMarkers(bool useTransaction = false, int num = 0);
    void benchmarkActions();

    inline QString createPolygonName( const int id ) { return QString( "polygon #%1" ).arg( id ); }
    inline QString createMarkerName( const int id ) { return QString( "marker #%1" ).arg( id ); }

private slots:
    void onOpenMap();
    void onCloseMap();

    void onZoomIn();
    void onZoomOut();
    void onMoveUp();
    void onMoveDown();
    void onMoveLeft();
    void onMoveRight();

    void onAddMarkers();
    void onAddPolygons();
    void onBenchmark();

    void onClearEdit();
    void onSaveEdit();

    void onBtnDeleteAllFeatures();
};

#endif // DEMOOBJECTSTIMER_H
