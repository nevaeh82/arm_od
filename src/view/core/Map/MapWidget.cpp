#include <QMessageBox>

#include "../Icons/IconsGenerator.h"

#include "MapWidget.h"
#include "ui_MapWidget.h"

MapWidget::MapWidget(QWidget *parent) :
	QWidget(parent)
	, ui(new Ui::MapWidget)
{
	ui->setupUi(this);

	connect( ui->_pwwidget, SIGNAL(mapClicked(double,double)), this, SIGNAL(mapClicked(double, double)) );

	m_xmlParser = new BaseParser(this);
    m_infoWidget = new MapInfoWidget(this);
    m_infoWidget->move(this->width()/2, 10);
    m_infoWidget->hide();


    m_movingTimer = new QTimer(this);
    connect(m_movingTimer, SIGNAL(timeout()), this, SLOT(onClearMovingAlarm()));

	connect(m_xmlParser->getView(), SIGNAL(signalBaseStationOnMap(double,double,QString)), this, SIGNAL(onShowBaseStation(double, double, QString)));
	connect(m_xmlParser->getView(), SIGNAL(signalClearStationOnMap()), this, SIGNAL(onClearBaseStation()));
}

MapWidget::~MapWidget()
{
	delete ui;
}

QWidget* MapWidget::getWidget()
{
	return this;
}

PwGisWidget* MapWidget::getPwGis()
{
	return ui->_pwwidget;
}

void MapWidget::slotApply(int id)
{
	emit signalApplyCross(id);
	//QMessageBox::critical(this, "income", "Ok", QMessageBox::Ok);
}

void MapWidget::slotLoadBaseStations()
{
    m_xmlParser->getView()->show();
}

void MapWidget::onSetSquareVal(QString val)
{
    if(val.isEmpty()) {
       if(m_infoWidget->styleSheet().isEmpty()) {
            m_infoWidget->setText("");
            m_infoWidget->hide();
            m_movingTimer->stop();
        }
        return;
    }

    m_infoWidget->setStyleSheet("");
    m_infoWidget->setText(val);
    m_infoWidget->show();

}

void MapWidget::onSetMovingAlarm(QString val)
{
    if(val.isEmpty()) {
        m_infoWidget->hide();
        m_infoWidget->setStyleSheet("");
        return;
    }

    if(val.isEmpty()) {
        m_infoWidget->hide();
        return;
    }

    m_infoWidget->setStyleSheet("border: 2px solid red; \
                                border-radius: 1; \
                                font: 75 14pt; \
                                color: rgb(255, 0, 0);" );
    m_infoWidget->setText(val);
    m_infoWidget->show();

    m_movingTimer->start(5000);
}

void MapWidget::onClearMovingAlarm()
{
    m_infoWidget->setStyleSheet("");
    m_infoWidget->hide();

    m_movingTimer->stop();
}
