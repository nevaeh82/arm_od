#ifndef IDBCONTROLLER_H
#define IDBCONTROLLER_H

#include <Settings/Object.h>
#include <Settings/Property.h>

class IDbController
{
public:
	virtual ~IDbController(){}

	virtual PropertiesList getProperties(const uint objectID) = 0;
	virtual Property getProperty(const uint propID) = 0;

	virtual ObjectsList getAllObjects() = 0;

	virtual uint addObject(const Object& object) = 0;
	virtual uint addProperty(const Property &property) = 0;

	virtual bool setPropertyValue(const uint propId, const QVariant propValue) = 0;
	virtual bool setPropertyState(const uint propId, const int state) = 0;
	virtual bool setPropertyEditable(const uint propId, const bool isEditable) = 0;

};

#endif // IDBCONTROLLER_H
