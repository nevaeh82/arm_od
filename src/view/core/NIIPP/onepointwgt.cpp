#include "onepointwgt.h"
#include "ui_onepointwgt.h"

onepointwgt::onepointwgt(int id, NStation cfg, MapController *map, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::onepointwgt),
    m_id(id),
    m_map(map)
{
    ui->setupUi(this);

    wgtadd = new onepointwidgetadditional(cfg, 0);

    setCurrentConfig(cfg);

    connect(ui->pbRm, SIGNAL(clicked(bool)), this, SLOT(gotoRemove()));

    connect(wgtadd, SIGNAL(paramChanged()), this, SLOT(stationChanged()));
    connect(ui->lineEdit, SIGNAL(editingFinished()), this, SLOT(stationChanged()));
    connect(ui->cbEnable, SIGNAL(clicked(bool)), this, SLOT(stationChanged()));
    connect(ui->cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(stationChanged()));
    connect(ui->pbSettings, SIGNAL(clicked(bool)), wgtadd, SLOT(show()));

    m_rebPixmap.load(":/images/RebAnt.png");
    m_pvoPixmap.load(":/images/PvoAnt.png");

    stationChanged();
}

onepointwgt::~onepointwgt()
{
    wgtadd->deleteLater();
    delete ui;
}

void onepointwgt::gotoRemove()
{
    m_map->removeStationPowerZone(getCurrentConfig());

    QSettings settings("./stations.ini",
                        QSettings::IniFormat);

    settings.beginGroup(QString("Station-%1").arg(m_id));
    settings.remove("");
    settings.endGroup();

    emit onRemove(m_id);
}

void onepointwgt::stationChanged()
{
    NStation cfg = getCurrentConfig();


    QSettings settings("./stations.ini",
                        QSettings::IniFormat);

    settings.beginGroup(QString("Station-%1").arg(cfg.getID()));
          settings.setValue("id", cfg.getID());
          settings.setValue("pos", QPointF(cfg.getPos().y(), cfg.getPos().x()));
          settings.setValue("azimuth", cfg.getAzimuth());
          settings.setValue("radius", cfg.getRadius()/1000);
          settings.setValue("width", cfg.getWidth());
          settings.setValue("name", cfg.getName());
          settings.setValue("isEnable", cfg.getIsEnable());
          settings.setValue("isCircle", cfg.getIsCircle());
          settings.setValue("type", (int)cfg.getMode());
          settings.setValue("isEnable", cfg.getIsEnable());
    settings.endGroup();

    m_map->updateStationPowerZone(cfg);

    if(cfg.getMode() == 0) {
        ui->lblPixmap->setPixmap(m_pvoPixmap);
    } else {
        ui->lblPixmap->setPixmap(m_rebPixmap);
    }
}

NStation onepointwgt::getCurrentConfig()
{
    NStation retVal;

    retVal = wgtadd->getParams();
    retVal.setID(m_id);
    retVal.setName(ui->lineEdit->text());

    retVal.setIsEnable(ui->cbEnable->isChecked());
    retVal.setMode(ui->cbType->currentIndex());

    return retVal;
}

void onepointwgt::setCurrentConfig(NStation param)
{
    ui->lineEdit->setText(param.getName());
    ui->cbEnable->setChecked(param.getIsEnable());
    ui->cbType->setCurrentIndex(param.getMode());
}
