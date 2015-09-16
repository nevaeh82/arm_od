#ifndef CONTROLPANELCONTROLLER_H
#define CONTROLPANELCONTROLLER_H

#include <QObject>
#include <QFileDialog>

#include "ControlPanel.h"
#include "Interfaces/IController.h"
#include "Interfaces/IUavHistory.h"
#include "Interfaces/IUavHistoryListener.h"

class ControlPanelController : public QObject, public IController<ControlPanel>, public IUavHistoryListener
{
	Q_OBJECT

private:
	ControlPanel* m_view;
	IUavHistory* m_uavHistory;

public:
	explicit ControlPanelController(QObject *parent = 0);

	void appendView(ControlPanel* view);

	void setUavHistory(IUavHistory* history);

	void onUavAdded(const Uav&, const QString&) {}
	void onUavRemoved(const Uav&, const QString&) {}
	void onUavInfoChanged(const UavInfo& uavInfo, const QString&,
						  const QVector<QPointF> &tail = QVector<QPointF>(),
						  const QVector<QPointF> &tailStdDev = QVector<QPointF>());
	void onStatusChanged(Status status);

signals:
	void historyStatusChanged(Status status);
	void currentDateTimeChanged(const QDateTime& date);

private slots:
	void changeViewStatus(Status status);
	void changeCurrentDateTime(const QDateTime& date);

	void onStartPlayingHistorySlot();
	void onStopPlayingHistorySlot();
	void onExportToXls();

	void onDbLogReceive(QString);

};

#endif // CONTROLPANELCONTROLLER_H
