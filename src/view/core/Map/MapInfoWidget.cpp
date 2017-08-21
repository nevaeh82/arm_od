#include "MapInfoWidget.h"
#include "ui_MapInfoWidget.h"

MapInfoWidget::MapInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapInfoWidget)
{
    ui->setupUi(this);
}

MapInfoWidget::~MapInfoWidget()
{
    delete ui;
}

void MapInfoWidget::setText(const QString &text)
{
    ui->label->setText(text);
    this->adjustSize();
}
