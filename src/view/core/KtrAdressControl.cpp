#include "KtrAdressControl.h"
#include "ui_KtrAdressControl.h"

#include <QSettings>
#include <QString>

KtrAdressControl::KtrAdressControl(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::KtrAdressControl)
{
	ui->setupUi(this);

	init();

	connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(slotApply()));

	ui->labelAlarm->setVisible(false);
}

KtrAdressControl::~KtrAdressControl()
{
	delete ui;
}

void KtrAdressControl::init()
{
	QSettings settings("./TCP/devices.ini", QSettings::IniFormat, this);

	QStringList childKeys = settings.childGroups();

	foreach (const QString &childKey, childKeys)
	{
		settings.beginGroup(childKey);
		QString name = settings.value("name", "Unknown").toString();
		if(name.contains("KTR"))
		{
			ui->ipframe->setText(settings.value("ip", "127.0.0.1").toString());
			ui->leBoardList->setText(settings.value("boardList", "").toString());

			settings.endGroup();
		   // m_ktrNmea->onConnect(m_KTRSettingStruct.host);

			return;
		}
		settings.endGroup();
	}
}

void KtrAdressControl::slotApply()
{
	QSettings settings("./TCP/devices.ini", QSettings::IniFormat, this);

	QStringList childKeys = settings.childGroups();

	foreach (const QString &childKey, childKeys)
	{
		settings.beginGroup(childKey);
		QString name = settings.value("name", "Unknown").toString();
		if(name.contains("KTR"))
		{
			settings.setValue("ip", QVariant(ui->ipframe->text()));
			settings.setValue("boardList", QVariant(ui->leBoardList->text()));

			settings.endGroup();

		   // m_ktrNmea->onConnect(m_KTRSettingStruct.host);
			ui->labelAlarm->setVisible(true);

			return;
		}
		settings.endGroup();
	}

	ui->labelAlarm->setVisible(true);
}
