#ifndef BASESTYLE_H
#define BASESTYLE_H

#include <QObject>
#include "PwGisStyle.h"

/**
 * @brief The BaseStyle class
 *	connect all new styles with PwGisStyle object
 */
class GISCLIENT_EXPORT BaseStyle : public QObject
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{92e651d8-f334-45df-a70a-169443428a8c}")
	Q_CLASSINFO("InterfaceID", "{60b93217-23b0-4e44-8d8f-d8c0756aa587}")
protected:
	PwGisStyle* _gisStyle;
	void applyToCss(PwGisStyle::Style propertyName, const QString& value);

public:
	explicit BaseStyle(PwGisStyle* aStyle, QObject *parent);
	explicit BaseStyle(QObject *parent);
};

#endif // BASESTYLE_H
