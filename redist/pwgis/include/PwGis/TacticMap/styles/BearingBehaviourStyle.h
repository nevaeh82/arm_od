#ifndef BEARINGDYNAMICSSTYLE_H
#define BEARINGDYNAMICSSTYLE_H

#include <QObject>
#include "../TacticMap_global.h"

class TACTICMAP_EXPORT BearingBehaviourStyle : public QObject
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{f153712d-cea6-478f-af39-9926db77cd16}")
	Q_CLASSINFO("InterfaceID", "{96926a5f-270d-4e74-addf-382d97b8fd95}")

	Q_PROPERTY (uint directionsLimit READ directionsLimit WRITE setDirectionsLimit)
	Q_PROPERTY (int directionLifetime READ directionLifetime WRITE setDirectionLifetime)

private:
	uint _directionsLimit;
	int _directionLifetime;

public:
	explicit BearingBehaviourStyle(QObject* parent = NULL);

	/**
	 * @brief Gets Directiosn limit
	 * @return count of max directions.
	 */
	uint directionsLimit() const;

	/**
	 * @brief Sets directions limit.
	 * @param directionsCount  count of displayed directions in one time..
	 */
	void setDirectionsLimit(uint directionsCount);

	/**
	 * @brief Gets direction  lifetime period.
	 * @return
	 */
	int directionLifetime() const;

	/**
	 * @brief Sets direction lifetime.
	 * @param Seconds amount which direction should be shown.
	 */
	void setDirectionLifetime(int seconds);

public slots:
	bool isDirectionsLimitDefined() const
	{
		return _directionsLimit != 0;
	}

	bool isDirectionLifetimeDefined() const
	{
		return _directionLifetime != 0;
	}


};

#endif // BEARINGDYNAMICSSTYLE_H
