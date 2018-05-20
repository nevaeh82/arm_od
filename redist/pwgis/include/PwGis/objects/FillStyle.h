#ifndef FILLSTYLE_H
#define FILLSTYLE_H

#include <QObject>
#include <QtGui/QColor>
#include "BaseStyle.h"

class PwGisStyle;

class GISCLIENT_EXPORT FillStyle : public BaseStyle
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{f38a20e0-edb5-4041-83a0-c5448a79563a}")
	Q_CLASSINFO("InterfaceID", "{1d058fe9-fce8-4a16-b812-c879179de4c4}")

	Q_PROPERTY (QColor color READ color WRITE setColor NOTIFY colorChanged)

	QColor _color;

public:
	explicit FillStyle(PwGisStyle* gisStyle, QObject *parent);
	explicit FillStyle(QObject *parent);

	QColor color() const
	{
		return _color;
	}
	void setColor(QColor color);

signals:

	void colorChanged(QColor color);

};

#endif // FILLSTYLE_H
