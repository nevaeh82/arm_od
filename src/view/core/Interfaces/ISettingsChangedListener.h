#ifndef ISETTINGSCHANGEDLISTENER_H
#define ISETTINGSCHANGEDLISTENER_H

class QString;

class ISettingsChangedListener
{

public:
	virtual ~ISettingsChangedListener(){}

	virtual void onSettingsChanged(const QString& category) = 0;
};



#endif // ISETTINGSCHANGEDLISTENER_H
