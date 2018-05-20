#ifndef IMAPOBJECTINFO_H
#define IMAPOBJECTINFO_H
#include <QString>

class QObject;

class IMapObjectInfo
{
public:
	/// Sets associated tag.
	/// @param tag  any object.
	virtual void setTag(QObject* tag)=0;

	/// Gets associated with object tag.
	/// @return Tag which was set via setTag().
	virtual QObject* tag()=0;

	/// Sets object name.
	virtual void setName(QString name)=0;

	/// Gets map object name.
	/// @return  Object name.
	virtual QString name()=0;
	
	virtual ~IMapObjectInfo(){}
};

#endif // IMAPOBJECTINFO_H
