#ifndef IGISCONTROL_H
#define IGISCONTROL_H
#include <QVariant>

class QString;
class QUrl;

namespace Pw {
namespace Gis {
namespace Client {

/**
 * @brief this interface IGisControl provides access to WebView
 */
class IGisControl {
public:
	virtual ~IGisControl(){}

	/**
	 * @brief Register Host object in WebView.
	 */
	virtual void registerHost()=0;

	/**
	 * @brief Set content to webview.
	 * @param content  html content which need to display.
	 */
	virtual void setWebContent(const QString& content)=0;

	/**
	 * @brief Load content from specified URL.
	 * @param url Resource location.
	 */
	virtual void loadContentFrom(QUrl& url)=0;

	/**
	 * @brief Execute custom script in webview.
	 * @param script  script code.
	 * @return script result
	 */
	virtual QVariant executeScript(const QString& script) = 0;
};

}
}
}
#endif // IGISCONTROL_H
