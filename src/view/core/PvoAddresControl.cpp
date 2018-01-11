#include "PvoAddresControl.h"
#include "ui_PvoAddresControl.h"

#include <QSettings>
#include <QString>

PvoAddressControl::PvoAddressControl(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::PvoAddressControl)
{
	ui->setupUi(this);

	init();

    connect(ui->pbApply, SIGNAL(clicked(bool)), this, SLOT(slotApply()));

    ui->lblWarning->hide();
}

PvoAddressControl::~PvoAddressControl()
{
	delete ui;
}

void PvoAddressControl::init()
{
    QSettings settings("./TCP/tcpPvoSettings.ini", QSettings::IniFormat, this);

    ui->ipframe->setText(settings.value("tcpClientHost", "127.0.0.1").toString());
    ui->sbClientPort->setValue(settings.value("tcpClientPort", 4444).toInt());
    ui->sbServerPort->setValue(settings.value("tcpServerPort", 5555).toInt());
    return;
}

void PvoAddressControl::slotApply()
{
    QSettings settings("./TCP/tcpPvoSettings.ini", QSettings::IniFormat, this);

    settings.setValue("tcpClientHost", ui->ipframe->text());
    settings.setValue("tcpClientPort", ui->sbClientPort->value());
    settings.setValue("tcpServerPort", ui->sbServerPort->value());

//    emit applySignal(ui->ipframe->text(), ui->sbClientPort->value(),
//                     ui->sbServerPort->value());
//   close();

    ui->lblWarning->show();
}
