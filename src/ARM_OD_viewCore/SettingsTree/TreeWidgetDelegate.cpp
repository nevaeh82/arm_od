#include "TreeWidgetDelegate.h"
#include <QtGui/QApplication>

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>

#include <QStylePainter>
#include <QMessageBox>

#include "UiDefines.h"

TreeWidgetDelegate::TreeWidgetDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{
	m_mode = DefaultMode;

	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TreeWidgetDelegate));
}

TreeWidgetDelegate::~TreeWidgetDelegate()
{

}

void TreeWidgetDelegate::setStationNamesList(const QStringList &stationsList)
{
	m_stationNamesList = stationsList;
}

QWidget *TreeWidgetDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	switch (m_mode) {
		case AveragingMode: {
			QSpinBox * editor = new QSpinBox(parent);
			editor->setRange(0, 100);
			connect(editor, SIGNAL(editingFinished()), this, SLOT(commitSBEditorSlot()));
			return editor;
		}
		case FrequencyMode: {
			QSpinBox * editor = new QSpinBox(parent);
			editor->setRange(20, 7000);
			connect(editor, SIGNAL(editingFinished()), this, SLOT(commitSBEditorSlot()));
			return editor;
		}
		case LeadingOPMode: {
			QComboBox* editor = new QComboBox(parent);

			for(int i = 0; i < m_stationNamesList.size(); ++i){
				editor->addItem(m_stationNamesList.at(i));
			}
			return editor;
		}

		case DefaultMode:
		default:
			return QStyledItemDelegate::createEditor(parent, option, index);
	}
}

void TreeWidgetDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	switch (m_mode) {
		case AveragingMode:
		case FrequencyMode: {
			QSpinBox * spControl = static_cast<QSpinBox*>(editor);
			int value = index.model()->data(index, Qt::EditRole).toInt();
			spControl->setValue(value);
			break;
		}
		case LeadingOPMode: {
			QComboBox* cbEditor = static_cast<QComboBox*>(editor);
			QString text = index.model()->data(index, Qt::EditRole).toString();
			int value = cbEditor->findText(text);
			cbEditor->setCurrentIndex(value);
			break;
		}
		case DefaultMode:
		default:
			QStyledItemDelegate::setEditorData(editor, index);
	}
}

void TreeWidgetDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	switch (m_mode) {
		case AveragingMode:
		case FrequencyMode: {
			QSpinBox * spControl = static_cast<QSpinBox*>(editor);
			model->setData(index, spControl->value(), Qt::EditRole);
			break;
		}
		case LeadingOPMode: {
			QComboBox* cbEditor = static_cast<QComboBox*>(editor);
			model->setData(index, cbEditor->currentText(), Qt::EditRole);
			break;
		}
		case DefaultMode:
		default:
			QStyledItemDelegate::setModelData(editor, model, index);
	}
}

void TreeWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (!index.isValid()) {
		return;
	}

	QStyledItemDelegate::paint(painter, option, index);
}

QSize TreeWidgetDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	/// Reserved for future
	return QStyledItemDelegate::sizeHint(option, index);
}

bool TreeWidgetDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	Q_UNUSED(event);
	Q_UNUSED(model);
	Q_UNUSED(option);

	QModelIndex ind = index.sibling(index.row(), 0);
	QString propName = ind.data(Qt::DisplayRole).toString();
	propName = getRealPropNameFromTranslate(propName);

	if(propName == DB_FREQUENCY_PROPERTY) {
		m_mode = FrequencyMode;
	} else if(propName == DB_AVERAGING_PROPERTY) {
		m_mode = AveragingMode;
	} else if(propName == DB_LEADING_OP_PROPERTY) {
		m_mode = LeadingOPMode;
	} else {
		m_mode = DefaultMode;
	}

	return false;
}


void TreeWidgetDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}


QString TreeWidgetDelegate::getRealPropNameFromTranslate(const QString& propName) const
{
	///DO NOT TOUCH
	QString temp1 = tr("Frequency");
	QString temp2 = tr("Averaging");
	QString temp3 = tr("Leading OP");

	if (propName == temp1) {
		return DB_FREQUENCY_PROPERTY;
	}
	else if (propName == temp2) {
		return DB_AVERAGING_PROPERTY;
	}
	else if (propName == temp3) {
		return DB_LEADING_OP_PROPERTY;
	}
	return propName;
}

QString TreeWidgetDelegate::getTranslatePropNameFromReal(const QString& propName) const
{
	///DO NOT TOUCH
	QString temp1 = tr("Frequency");
	QString temp2 = tr("Averaging");
	QString temp3 = tr("Leading OP");

	if (propName == DB_FREQUENCY_PROPERTY) {
		return temp1;
	}
	else if (propName == DB_AVERAGING_PROPERTY) {
		return temp2;
	}
	else if (propName == DB_LEADING_OP_PROPERTY) {
		return temp3;
	}

	return propName;
}

void TreeWidgetDelegate::commitSBEditorSlot()
{
	QSpinBox* editor = qobject_cast<QSpinBox *>(sender());
	emit commitData(editor);
}
