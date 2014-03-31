#ifndef TREEWIDGETDELEGATE_H
#define TREEWIDGETDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QPainter>
#include <QPushButton>
#include <QMutex>
#include <QMutexLocker>

#include <PwLogger/PwLogger.h>

class TreeWidgetDelegate : public QStyledItemDelegate
{
	Q_OBJECT

private:
	typedef enum UiMode
	{
		DefaultMode			= 0,
		FrequencyMode		= 2,
		AveragingMode		= 3,
		LeadingOPMode		= 4
	}UiMode;

	UiMode m_mode;

	QStringList m_stationNamesList;

	QMutex m_stateMutex;

	Pw::Logger::ILogger* m_logger;

public:
	TreeWidgetDelegate(QObject *parent = 0);
	virtual ~TreeWidgetDelegate();

	void setStationNamesList(const QStringList& stationsList);

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const;

private:
	QString getRealPropNameFromTranslate(const QString& propName) const;
	QString getTranslatePropNameFromReal(const QString& propName) const;

signals:
	void connectToDeviceSignal(const QString& deviceName);
	void disconnectFromDeviceSignal(const QString& deviceName);

private slots:
	void commitSBEditorSlot();

};

#endif // TREEWIDGETDELEGATE_H
