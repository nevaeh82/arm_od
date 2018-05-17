#ifndef RADIOSOURCE_H
#define RADIOSOURCE_H
#include "TracedObjectAbstract.h"
#include "TacticMap_global.h"

class RadioSourceStyle;

/**
 * @brief The RadioSource class presents Radio source object on map.
 */
class TACTICMAP_EXPORT RadioSource: public TracedObjectAbstract
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{55f183a2-0b59-406f-840d-7221ffe52fc5}")
	Q_CLASSINFO("InterfaceID", "{589505f2-1bc5-4d12-9aff-a96058b994a7}")

private:
	RadioSourceStyle* _style;

public:
	explicit RadioSource(IObjectsFactory* factory, QString id, QString name = "", QObject* parent = NULL);
	explicit RadioSource(QObject* parent=0) : TracedObjectAbstract(parent)
	{
	}

	virtual ~RadioSource();
public slots:
	void setStyle(RadioSourceStyle* style);
};

#endif // RadioSource_H
