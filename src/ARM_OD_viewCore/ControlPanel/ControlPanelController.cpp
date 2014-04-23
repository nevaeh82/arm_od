#include <QMessageBox>
#include <QApplication>

#include <Logger.h>

#include "ControlPanelController.h"
#include "Tabs/DbBla/Defines.h"

ControlPanelController::ControlPanelController(QObject *parent) :
	QObject(parent)
{
	m_view = NULL;
	m_uavHistory = NULL;
	m_isStarted = false;

	connect( this, SIGNAL(historyStatusChanged(Status)), SLOT(changeViewStatus(Status)) );
}

void ControlPanelController::appendView(ControlPanel *view)
{
	m_view = view;

	m_view->setStartDateTime( QDateTime::currentDateTime().addSecs( -7200 ) );
	m_view->setEndDateTime( QDateTime::currentDateTime() );

	connect(m_view, SIGNAL(startPlayingHistorySignal()), this, SLOT(onStartPlayingHistorySlot()));
	connect(m_view, SIGNAL(stopPlayingHistorySignal()), this, SLOT(onStopPlayingHistorySlot()));
}

void ControlPanelController::setUavHistory(IUavHistory *history)
{
	m_uavHistory = history;
	onStatusChanged( m_uavHistory->getStatus() );
}

void ControlPanelController::onUavInfoChanged(const UavInfo& uavInfo, const QString&)
{
	m_view->setCurrentDateTime( uavInfo.dateTime );
}

void ControlPanelController::onStatusChanged(IUavHistoryListener::Status status)
{
	emit historyStatusChanged(status);
}

void ControlPanelController::changeViewStatus(IUavHistoryListener::Status status)
{
	QApplication::restoreOverrideCursor();

	switch( status ) {
		case IUavHistoryListener::Loading:
			QApplication::setOverrideCursor(Qt::BusyCursor);

		case IUavHistoryListener::NotReady:
			m_view->setEnabled( false );
			break;

		case IUavHistoryListener::Ready:
			m_view->setEnabled( true );
			m_view->setPlayingEnabled( false );
			break;

		case IUavHistoryListener::Playing:
			m_view->setEnabled( true );
			m_view->setPlayingEnabled( true );
			break;
	}
}

void ControlPanelController::onStartPlayingHistorySlot()
{
	if (NULL == m_uavHistory) {
		log_debug("m_uavHistory is NULL");
		return;
	}

	if( m_view->getStartDateTime() > m_view->getEndDateTime() ) {
		QMessageBox::warning(m_view, tr("Unable to play history"), tr("End date should be greater than start date."));
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
