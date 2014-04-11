#include "TreeWidgetDelegate.h"
#include <QtGui/QApplication>
#include <QDebug>
#include <QFile>

#include <QLabel>

#include <QCheckBox>
#include <QDir>
#include <QComboBox>
#include <QTextEdit>
#include <QSpinBox>


TreeWidgetDelegate::TreeWidgetDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{
	_pm_round_red = new QPixmap(":/images/signals/images/bullet_red.png");
	_pm_round_green = new QPixmap(":/images/signals/images/bullet_green.png");

	QDir dir;
	dir = QDir::currentPath();

	QFile file("Styles/stylesheet.qcss");
	file.open(QFile::ReadOnly);
	if(file.isOpen())
	{
		_styleSheet = QLatin1String(file.readAll());
		file.close();
	}
}

void TreeWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();

	QVariant var = index.model()->data(index, Qt::CheckStateRole);
	QString text = index.model()->data(index, Qt::DisplayRole).toString();

	if(!var.isNull()) {
		bool checked = index.model()->data(index, Qt::CheckStateRole).toBool();
		QCheckBox renderer;
		renderer.setChecked(checked);
		renderer.setStyleSheet(_styleSheet);
		renderer.setText(text);
		renderer.resize(option.rect.size());
		painter->translate(option.rect.topLeft());
		renderer.render(painter);
	} else {
		QLabel renderer;
		renderer.setStyleSheet(_styleSheet);
		renderer.setText(text);
		renderer.resize(option.rect.size());
		painter->translate(option.rect.topLeft());
		renderer.render(painter);
	}

	painter->restore();
}
