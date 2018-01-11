#include "onepointwidgetadditional.h"
#include "ui_onepointwidgetadditional.h"

onepointwidgetadditional::onepointwidgetadditional(NStation cfg, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::onepointwidgetadditional)
{
    ui->setupUi(this);

    setParams(cfg);

    connect(ui->sbAzimuth, SIGNAL(valueChanged(int)), this, SIGNAL(paramChanged()));
    connect(ui->sbRadius, SIGNAL(valueChanged(int)), this, SIGNAL(paramChanged()));
    connect(ui->sbWidth, SIGNAL(valueChanged(int)), this, SIGNAL(paramChanged()));
    connect(ui->cbRoundView, SIGNAL(clicked(bool)), this, SIGNAL(paramChanged()));
    connect(ui->latSb, SIGNAL(valueChanged(double)), this, SIGNAL(paramChanged()));
    connect(ui->lonSB, SIGNAL(valueChanged(double)), this, SIGNAL(paramChanged()));
}

onepointwidgetadditional::~onepointwidgetadditional()
{
    delete ui;
}

NStation onepointwidgetadditional::getParams()
{
    NStation retVal;
    retVal.setAzimuth(ui->sbAzimuth->value());
    retVal.setIsCircle(ui->cbRoundView->isChecked());
    retVal.setPos(QPointF(ui->latSb->value(), ui->lonSB->value()));
    retVal.setRadius(ui->sbRadius->value()*1000);
    retVal.setWidth(ui->sbWidth->value());

    return retVal;
}

void onepointwidgetadditional::setParams(NStation cfg)
{
    ui->latSb->setValue(cfg.getPos().y());
    ui->lonSB->setValue(cfg.getPos().x());
    ui->sbAzimuth->setValue(cfg.getAzimuth());
    ui->cbRoundView->setChecked(cfg.getIsCircle());
    ui->sbRadius->setValue(cfg.getRadius());
    ui->sbWidth->setValue(cfg.getWidth());
}

