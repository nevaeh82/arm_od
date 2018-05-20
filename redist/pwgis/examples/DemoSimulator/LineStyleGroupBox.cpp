#include "LineStyleGroupBox.h"
#include "ui_LineStyleGroupBox.h"

#include <PwGis/objects/LineStyle.h>
#include <PwGis/TacticMap/TracedObjectAbstract.h>
#include <QColorDialog>

LineStyleGroupBox::LineStyleGroupBox(QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::LineStyleGroupBox),
	_currentItem(0)
{
	ui->setupUi(this);

	ui->dashStyleComboBox->addItem("Solid", LineStyle::Solid);
	ui->dashStyleComboBox->addItem("Dot", LineStyle::Dot);
	ui->dashStyleComboBox->addItem("Dash", LineStyle::Dash);
	ui->dashStyleComboBox->addItem("Dash-dot", LineStyle::DashDot);
	ui->dashStyleComboBox->addItem("Long dash", LineStyle::LongDash);
	ui->dashStyleComboBox->addItem("Long dash-dot", LineStyle::LongDashDot);

	connect(ui->btnColorSelect,SIGNAL(clicked()),SLOT(onColorBtnClicked()));
	connect(ui->dashStyleComboBox,SIGNAL(currentIndexChanged(int)),SLOT(onDashStyleChanged(int)));
	connect(ui->lineSizeComboBox,SIGNAL(currentIndexChanged(QString)),SLOT(onLineSizeChanged(QString)));
}

LineStyleGroupBox::~LineStyleGroupBox()
{
	delete ui;
}

void LineStyleGroupBox::setProvider(IMapProvider* provider)
{
	_mapProvider = provider;
}

void LineStyleGroupBox::setItem(TracedObjectAbstract* item)
{
	_currentItem = item;
	if(item == NULL){
		return;
	}

	QColor color = _currentItem->pathStyle()->line()->color();
	setButtonColor(color);

	int size = _currentItem->pathStyle()->line()->thickness();
	ui->lineSizeComboBox->setCurrentIndex(size - 1);

	LineStyle::LineDashStyle style = _currentItem->pathStyle()->line()->dashStyle();
	selectDashStyle(style);
}

void LineStyleGroupBox::onColorBtnClicked()
{
	QColor col = QColorDialog::getColor(Qt::white, this);
	if(col.isValid()) {
		setButtonColor(col);

		if( !_currentItem ){
			return;
		}

		_currentItem->pathStyle()->line()->setColor(col);
        _currentItem->updateMap();
	}
}

void LineStyleGroupBox::onDashStyleChanged(int index)
{
	if( !_currentItem ){
		return;
	}

	LineStyle::LineDashStyle dashStyle = (LineStyle::LineDashStyle) ui->dashStyleComboBox->itemData(index).toInt();

	_currentItem->pathStyle()->line()->setDashStyle(dashStyle);
    _currentItem->updateMap();
}

void LineStyleGroupBox::onLineSizeChanged(QString size)
{
	if( !_currentItem ){
		return;
	}

	_currentItem->pathStyle()->line()->setThickness(size.toInt());
    _currentItem->updateMap();
}

void LineStyleGroupBox::setButtonColor(QColor color)
{
	QColor textColor = QColor(255-color.red(),255-color.green(),255-color.blue());

	QString qss = QString("background-color: %1; color: %2").arg(color.name()).arg(textColor.name());
	ui->btnColorSelect->setStyleSheet(qss);
}

void LineStyleGroupBox::selectDashStyle(int style)
{
	int index = ui->dashStyleComboBox->findData(style);
	if(index == -1){
		return;
	}
	ui->dashStyleComboBox->setCurrentIndex(index);
}
