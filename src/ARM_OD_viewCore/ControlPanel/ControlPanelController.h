#ifndef CONTROLPANELCONTROLLER_H
#define CONTROLPANELCONTROLLER_H

#include <QObject>

#include "Interfaces/IController.h"
#include "ControlPanel.h"
#include "Interfaces/IUavHistory.h"

class ControlPanelController : public QObject, public IController<ControlPanel>
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

signals:

private slots:

	void onStartPlayingHistorySlot();
	void onStopPlayingHistorySlot();

};

#endif // CONTROLPANELCONTROLLER_H
