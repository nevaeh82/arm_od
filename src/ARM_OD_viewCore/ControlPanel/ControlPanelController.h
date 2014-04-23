#ifndef CONTROLPANELCONTROLLER_H
#define CONTROLPANELCONTROLLER_H

#include <QObject>

#include "ControlPanel.h"
#include "Interfaces/IController.h"
#include "Interfaces/IUavHistory.h"
#include "Interfaces/IUavHistoryListener.h"

class ControlPanelController : public QObject, public IController<ControlPanel>, public IUavHistoryListener
{
	Q_OBJECT

private:
	ControlPanel* m_view;

	bool m_isStarted;

	IUavHistory* m_uavHistory;

public:
	explicit ControlPanelController(QObject *parent = 0);

	void appendView(ControlPanel* view);

	void setUavHistory(IUavHistory* history);

	void onUavAdded(const Uav&, const QString&) {}
	void onUavRemoved(const Uav&, const QString&) {}
	void onUavInfoChanged(const UavInfo& uavInfo, const QString&);
	void onStatusChanged(Status status);

signals:
	void historyStatusChanged(Status status);

private slots:
	void changeViewStatus(Status status);

	void onStartPlayingHistorySlot();
	void onStopPlayingHistorySlot();

};

#endif // CONTROLPANELCONTROLLER_H
