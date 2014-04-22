#include "ControlPanelController.h"

#include <QMessageBox>


#include <Logger.h>

ControlPanelController::ControlPanelController(QObject *parent) :
	QObject(parent)
{
	m_view = NULL;
	m_uavHistory = NULL;
	m_isStarted = false;
}

void ControlPanelController::appendView(ControlPanel *view)
{
	m_view = view;

	connect(m_view, SIGNAL(startPlayingHistorySignal()), this, SLOT(onStartPlayingHistorySlot()));
	connect(m_view, SIGNAL(stopPlayingHistorySignal()), this, SLOT(onStopPlayingHistorySlot()));
}

void ControlPanelController::setUavHistory(IUavHistory *history)
{
	m_uavHistory = history;
}

void ControlPanelController::onStartPlayingHistorySlot()
{
	if (NULL == m_uavHistory) {
		log_debug("m_uavHistory is NULL");
		return;
	}

	if (!m_uavHistory->start(m_view->getStartDateTime(), m_view->getEndDateTime())) {
		QMessageBox::warning(m_view, tr("Unable to play history"), tr("Data for selected period do not exist."));
		return;
	}

	m_isStarted = true;

}

void ControlPanelController::onStopPlayingHistorySlot()
{
	if (NULL == m_uavHistory) {
		log_debug("m_uavHistory is NULL");
		return;
	}

	m_uavHistory->stop();

	m_isStarted = false;
}
