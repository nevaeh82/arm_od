#include "BaseParserWidget.h"
#include "ui_BaseParserWidget.h"

#include <QFileDialog>

#include <QApplication>

BaseParserWidget::BaseParserWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::BaseParserWidget)
{
	ui->setupUi(this);

	connect(ui->pbOpen, SIGNAL(clicked()), this, SLOT(slotOpenXmpPath()));
	connect(ui->pbMapClear, SIGNAL(clicked(bool)), this, SIGNAL(signalClearStationOnMap()));

}

BaseParserWidget::~BaseParserWidget()
{
	delete ui;
}

void BaseParserWidget::setTableData(const QList<MobileSession>& data)
{
	int row = 0;

	ui->tableWidget->clearContents();

	ui->tableWidget->setRowCount(data.size());
	ui->tableWidget->setColumnWidth(1, 200);
	ui->tableWidget->setColumnWidth(2, 100);
	for(int i = 0; i<data.size(); i++) {
		for(int j = 0; j<8; j++) {
			ui->tableWidget->setItem(i, j, new QTableWidgetItem(""));
		}
		ui->tableWidget->setRowHeight(i, 60);
	}

	foreach (MobileSession session, data) {
		ui->tableWidget->item(row, 0)->setText(session.standart);
		ui->tableWidget->item(row, 1)->setText(session.datetime + "\n" + session.duration + tr("sec"));
		ui->tableWidget->item(row, 2)->setText(session.network + "\n" + session.basestation);
		ui->tableWidget->item(row, 3)->setText(session.lac + "\n" + session.cid);
		ui->tableWidget->item(row, 4)->setText(session.frequency + tr("mHz"));
		ui->tableWidget->item(row, 5)->setText(QString::number(session.lon));
		ui->tableWidget->item(row, 6)->setText(QString::number(session.lat));
		ui->tableWidget->item(row, 7)->setText(session.source);

		if(ui->cbOnMap->isChecked()) {
			emit signalBaseStationOnMap(session.lon, session.lat, session.source);
		}

		row++;

		if(row >= 100000) {
			return;
		}

		qApp->processEvents();
	}
}

void BaseParserWidget::slotOpenXmpPath()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Open Database files"), "");
	ui->lePath->setText(path);
	emit signalPathSelected(path);
}
