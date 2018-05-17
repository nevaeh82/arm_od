#ifndef TEXTSTYLE_H
#define TEXTSTYLE_H

#include <QObject>
#include <QtGui/QColor>
#include "BaseStyle.h"

class GISCLIENT_EXPORT TextStyle : public BaseStyle
{
	//TODO: Map TextStyle! Warning it's not working now
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{5bd451e9-32f0-4fb5-a726-da7519b2e74b}")
	Q_CLASSINFO("InterfaceID", "{5155a2f9-c341-4190-9ad5-056273acb3b3}")

	Q_PROPERTY (QColor  backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
	Q_PROPERTY (QColor foregroundColor READ foregroundColor WRITE setForegroundColor NOTIFY foregroundColorChanged)
	Q_PROPERTY (QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
	Q_PROPERTY (QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
	Q_PROPERTY (unsigned long fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

	QString _fontFamily;

	QColor _backgroundColor;

	QColor _foregroundColor;

	QColor _borderColor;

	unsigned long  _fontSize;

public:
	explicit TextStyle(PwGisStyle* style, QObject *parent);
	explicit TextStyle(QObject *parent);


	QColor backgroundColor() const
	{
		return _backgroundColor;
	}

	QColor foregroundColor() const
	{
		return _foregroundColor;
	}

	QColor borderColor() const
	{
		return _borderColor;
	}

	QString fontFamily() const
	{
		return _fontFamily;
	}

	unsigned long fontSize() const
	{
		return _fontSize;
	}

	void setBackgroundColor(QColor  color);
	void setForegroundColor(QColor color);
	void setBorderColor(QColor color);

	void setFontFamily(QString fontFamily);
	void setFontSize(unsigned long arg);

signals:

	void backgroundColorChanged(QColor color);

	void foregroundColorChanged(QColor color);

	void borderColorChanged(QColor color);

	void fontFamilyChanged(QString fontFamily);

	void fontSizeChanged(unsigned long arg);

};

#endif // TEXTSTYLE_H
