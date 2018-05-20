#ifndef LINESTYLE_H
#define LINESTYLE_H

#include <QObject>
#include <QtGui/QColor>
#include "BaseStyle.h"

class GISCLIENT_EXPORT LineStyle : public BaseStyle
{
	Q_OBJECT

	Q_ENUMS(LineDashStyle)
	Q_CLASSINFO("ClassID", "{0e9dcb54-b345-4b74-bcde-4d645b73dee6}")
	Q_CLASSINFO("InterfaceID", "{9d054db3-09c1-4a87-a62f-8cf3e8c3dc88}")

	Q_PROPERTY (QColor color READ color WRITE setColor NOTIFY colorChanged)
	Q_PROPERTY (unsigned int  thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)
	Q_PROPERTY (QColor outLineColor READ outLineColor WRITE setOutLineColor NOTIFY colorChanged)
	Q_PROPERTY (LineDashStyle dashStyle READ dashStyle WRITE setDashStyle NOTIFY dashStyleChanged)

public:
	enum LineDashStyle{
		Solid,
		Dot,
		Dash,
		DashDot,
		LongDash,
		LongDashDot
	};

private:

	unsigned int _thickness;
	QColor _color;
	QColor _outLineColor;
	LineDashStyle _dashStyle;

public:
	explicit LineStyle(PwGisStyle* style, QObject *parent);
	explicit LineStyle(QObject *parent);

	QColor color() const
	{
		return _color;
	}

	//Not working now
	QColor outLineColor() const
	{
		return _outLineColor;
	}

	unsigned int thickness() const
	{
		return _thickness;
	}

	LineDashStyle dashStyle() const
	{
		return _dashStyle;
	}

	void setDashStyle(LineDashStyle dash);

	void setThickness(unsigned int thickness);

	void setColor(QColor color);
	void setOutLineColor(QColor color);

signals:

	void thicknessChanged(unsigned int  thickness);

	void colorChanged(QColor color);
	void outLineColorChanged(QColor color);
	void dashStyleChanged(LineDashStyle style);

};

#endif // LINESTYLE_H
