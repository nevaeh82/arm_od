#ifndef IDBCHANGEDLISTENER_H
#define IDBCHANGEDLISTENER_H

#include "Settings/SettingsNode.h"

class IDbChangedListener
{
public:
	virtual ~IDbChangedListener(){}

	virtual void onSettingsNodeChanged(const SettingsNode&) = 0;
	virtual void onPropertyChanged(const Property&) = 0;
	virtual void onCleanSettings() = 0;
};

#endif // IDBCHANGEDLISTENER_H
