#ifndef ICONSTYLE_H
#define ICONSTYLE_H

#include <QObject>
#include "BaseStyle.h"
#include <QSize>

class GISCLIENT_EXPORT IconStyle : public BaseStyle
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{afa2cdaf-7c73-43ff-98ce-0ce17d167f60}")
	Q_CLASSINFO("InterfaceID", "{dcfb7898-46a8-4fba-8482-69807f775519}")

	Q_PROPERTY (QString iconPath READ iconPath WRITE setIconPath NOTIFY iconPathChanged)
	Q_PROPERTY (bool showIcon READ showIcon WRITE setShowIcon NOTIFY showIconChanged)
	Q_PROPERTY (QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)

	QString _iconName;

	bool _showIcon;

	QSize _iconSize;

public:
	explicit IconStyle(PwGisStyle* style,QObject *parent);
	explicit IconStyle(QObject *parent);

	QString iconPath() const
	{
		return _iconName;
	}

	bool showIcon() const
	{
		return _showIcon;
	}

	QSize iconSize() const
	{
		return _iconSize;
	}

	void setIconPath(QString iconPath);
	void setShowIcon(bool showIcon);
	void setIconSize(QSize iconSize);

signals:

	void iconPathChanged(QString arg);

	void showIconChanged(bool arg);

	void iconSizeChanged(QSize arg);

};

#endif // ICONSTYLE_H
