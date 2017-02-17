#include "FormCapture.h"
#include "ui_FormCapture.h"

FormCapture::FormCapture(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormCapture)
{
	ui->setupUi(this);

	connect(ui->pbApply, SIGNAL(clicked(bool)), this, SLOT(slotApply()));
	connect(ui->pbClear, SIGNAL(clicked(bool)), this, SIGNAL(signalClear()));
}

FormCapture::~FormCapture()
{
	delete ui;
}

void FormCapture::slotApply()
{
	emit signalApply(ui->spinBox->value());
}
