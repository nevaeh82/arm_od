#ifndef GISCONTROL_H
#define GISCONTROL_H

#include <QObject>
#include "IGisControl.h"

class PwGisWidget;
class QWebView ;

namespace Pw { namespace Logger {
class ILogger;
}}

namespace Pw {
namespace Gis {
namespace Client {

/**
 * @brief The GisControl class is implementation of  IGisControl
 */
class GisControl : public QObject, public IGisControl
{
	Q_OBJECT
private:
	PwGisWidget *_gis;
	QWebView *_webview;
	Pw::Logger::ILogger* _logger;

public:
	explicit GisControl(QObject *parent, PwGisWidget *gis);

signals:

public slots:
	/**
	 * @brief Register Host object in WebView.
	 */
	virtual void registerHost();

	/**
	 * @brief Set content to webview.
	 * @param content  html content which need to display.
	 */
	virtual void setWebContent(const QString& content);


	/**
	 * @brief Load content from specified URL.
	 * @param url Resource location.
	 */
	virtual void loadContentFrom(QUrl& url);

	/**
	 * @brief Set webview which should be used.
	 * @param view
	 */
	void setWebView(QWebView* view);


	/**
	 * @brief Execute custom script in webview.
	 * @param script  script code.
	 */
	QVariant executeScript(const QString& script);
};

}}}

#endif // GISCONTROL_H
