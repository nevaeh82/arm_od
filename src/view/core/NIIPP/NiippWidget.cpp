#include <Tcp/BaseTcpClient.h>

#include "NiippWidget.h"
#include "ui_NiippWidget.h"

NiippWidget::NiippWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::NiippWidget)
{
	ui->setupUi(this);

    m_cnt = 0;

    connect(ui->pbAdd, SIGNAL(clicked(bool)), this, SLOT(setMouse(bool)));
}

NiippWidget::~NiippWidget()
{
    delete ui;
}

void NiippWidget::loadStations()
{
    QSettings settings("./stations.ini",
                        QSettings::IniFormat);

    QStringList groupList = settings.childGroups();

    foreach (QString group, groupList) {
        NStation cfg;
        settings.beginGroup(QString(group));
        cfg.setID(settings.value("id").toInt());
        QPointF pos = settings.value("pos").toPointF();
        cfg.setPos(QPointF(pos.x(), pos.y()));
        cfg.setAzimuth(settings.value("azimuth").toInt());
        cfg.setRadius(settings.value("radius").toInt());
        cfg.setWidth(settings.value("width").toInt());
        cfg.setName(settings.value("name").toString());
        cfg.setIsEnable(settings.value("isEnable").toBool());
        cfg.setIsCircle(settings.value("isCircle").toBool());
        cfg.setMode(settings.value("type").toInt());
        cfg.setIsEnable(settings.value("isEnable").toBool());
        settings.endGroup();

        m_cnt = qMax(m_cnt, cfg.getID());

        onAddStation(cfg, cfg.getID());
    }
}


void NiippWidget::setMap(MapController *map)
{
    m_map = map;
    connect(m_map, SIGNAL(mapClicked(double,double)), this, SLOT(onAddStation(double, double)));
    connect(m_map, SIGNAL(mapOpened()), this, SLOT(loadStations()));
}

void NiippWidget::onAddStation(double lon, double lat)
{
    if(ui->pbAdd->isChecked()) {
        m_cnt++;

        NStation cfg;
        cfg.setPos(QPointF(lon, lat));
        cfg.setName(tr("Station-%1").arg(m_cnt));

        onepointwgt *wgt = new onepointwgt(m_cnt, cfg, m_map, this);
        ui->verticalLayout_2->insertWidget(ui->verticalLayout_2->count()-1, wgt);
        m_stationMap.insert(m_cnt , wgt);

        connect(wgt, SIGNAL(onRemove(int)), this, SLOT(stationRemove(int)));

        ui->pbAdd->setChecked(false);
        QApplication::overrideCursor()->setShape(Qt::ArrowCursor);
    }
}

void NiippWidget::onAddStation(NStation cfg, int id)
{
    //if(ui->pbAdd->isChecked()) {

        onepointwgt *wgt = new onepointwgt(id, cfg, m_map, this);
        ui->verticalLayout_2->insertWidget(ui->verticalLayout_2->count()-1, wgt);
        m_stationMap.insert(id , wgt);

        connect(wgt, SIGNAL(onRemove(int)), this, SLOT(stationRemove(int)));

        ui->pbAdd->setChecked(false);
        //QApplication::overrideCursor()->setShape(Qt::ArrowCursor);
    //}
}

void NiippWidget::stationRemove(int id)
{
     onepointwgt* wgt = m_stationMap.take(id);
     if(wgt) {
         ui->verticalLayout_2->removeWidget(wgt);
         wgt->close();
         delete wgt;
     }
}

void NiippWidget::setMouse(bool val)
{
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    if(val) {
        QApplication::overrideCursor()->setShape(Qt::CrossCursor);
    } else {
        QApplication::overrideCursor()->setShape(Qt::ArrowCursor);
    }
}
