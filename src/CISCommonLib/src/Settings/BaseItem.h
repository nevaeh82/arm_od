#ifndef BASEITEM_H
#define BASEITEM_H

#include <QString>
#include <QVariant>

enum BaseItemType
{
	TypeObject,
	TypeProperty,
	TypeUnknown
};

typedef struct BaseItem
{
	BaseItem()
	{
		id = 0;
		pid = 0;
		name = QString();
		value = QVariant();
		state = 0;
		isEditable = false;
	}

	int id;
	int pid;
	QString name;
	QVariant value;
	int state;
	bool isEditable;

protected:
	BaseItemType type;

}BaseItem;

#endif // BASEITEM_H
