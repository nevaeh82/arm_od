#ifndef GEOUTILS_H
#define GEOUTILS_H
#include <QString>

class GeoUtils
{
private:
	/// coordinates convertation presicion.
	enum {
		c_coordinatePrecision = 40
	};

public:

	static QString coordinate( double value)
	{
		return QString::number(value, 'g', (int)c_coordinatePrecision);
	}
};

#endif // GEOUTILS_H
