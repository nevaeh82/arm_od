#ifndef MAPMANAGEREVENTS_H
#define MAPMANAGEREVENTS_H

#include <QObject>
#include "pwgiswidget.h"

namespace Pw { namespace Gis {

class GISCLIENT_EXPORT MapManagerEvents : public QObject
{
	Q_OBJECT
	Q_CLASSINFO("ClassID","{e6f779bd-892f-481f-ab15-de2c8836b909}")
	Q_CLASSINFO("InterfaceID","{efa32c39-a40d-408f-b3a8-061dadcbf336}")
	Q_CLASSINFO("EventsID","{f661d98d-2a3d-4a5b-83af-5852966fb70f}")

public:
	explicit MapManagerEvents(QObject *parent = 0);
	virtual ~MapManagerEvents();

signals:
	///  Signal Caused when map successfully loaded to widget.
	void mapReady();

	///  Signal Caused when atlas successfully loaded to widget.
	void atlasReady();

	/// Signal redirected from void QWebView::loadProgress ( int progress )
	/// @param  progress  in % from 0 to 100.
	void loadProgress ( int progress );

	/// @brief  Signal emitted when widget status changed.
	///
	/// @param progressStatus  progress step.
	/// @param isSuccess  success flag , contains true if current step finished successes.
	void progress( PwGisWidget::Progress progressStatus, bool isSuccess);

	/**
	 * @brief Event emitted by user (could be used from JavaScript)
	 * @param arguments arguments, passed by user
	 */
	void userEvent( const QVariantList arguments = QVariantList() );

protected slots:
	/// Internal slot for browser.
	virtual void loadWebViewStarted();
	virtual void loadWebViewFinished( bool, const QUrl& );

};

}}

#endif // MAPMANAGEREVENTS_H
