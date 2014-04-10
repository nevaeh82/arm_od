#ifndef TCPAISZONEDEFINES_H
#define TCPAISZONEDEFINES_H

#define EUROPE_LATITUDE_MIN			33.57
#define EUROPE_LATITUDE_MAX			72.57
#define EUROPE_LONGITUDE_MIN		-16.96
#define EUROPE_LONGITUDE_MAX		55.05

#define NORTHAMERICA_LATITUDE_MIN		3.0
#define NORTHAMERICA_LATITUDE_MAX		75.0
#define NORTHAMERICA_LONGITUDE_MIN		-180.0
#define NORTHAMERICA_LONGITUDE_MAX		-52.0

#define SOUTHAMERICA_LATITUDE_MIN		-57.0
#define SOUTHAMERICA_LATITUDE_MAX		16.0
#define SOUTHAMERICA_LONGITUDE_MIN		-96.0
#define SOUTHAMERICA_LONGITUDE_MAX		-31.0

#define OCEANIA_LATITUDE_MIN		-55.08
#define OCEANIA_LATITUDE_MAX		19.62
#define OCEANIA_LONGITUDE_MIN		88.40
#define OCEANIA_LONGITUDE_MAX		180.0

#define ASIA_LATITUDE_MIN		12.48
#define ASIA_LATITUDE_MAX		79.98
#define ASIA_LONGITUDE_MIN		40.91
#define ASIA_LONGITUDE_MAX		179.77

#define AFRICA_LATITUDE_MIN			-39.0
#define AFRICA_LATITUDE_MAX			39.0
#define AFRICA_LONGITUDE_MIN		-29.0
#define AFRICA_LONGITUDE_MAX		55.0

#define ATLANTIC_LATITUDE_MIN			15.62
#define ATLANTIC_LATITUDE_MAX			52.69
#define ATLANTIC_LONGITUDE_MIN			-50.90
#define ATLANTIC_LONGITUDE_MAX			-4.75

#define MALDIVES_LATITUDE_MIN			-6.08
#define MALDIVES_LATITUDE_MAX			10.72
#define MALDIVES_LONGITUDE_MIN			63.10
#define MALDIVES_LONGITUDE_MAX			86.53

#define NORTHATLANTIC_LATITUDE_MIN			59.02
#define NORTHATLANTIC_LATITUDE_MAX			82.62
#define NORTHATLANTIC_LONGITUDE_MIN			-84.53
#define NORTHATLANTIC_LONGITUDE_MAX			4.45

typedef struct AISZone
{
	AISZone()
	{
		latitudeMax		= 0.0;
		longitudeMin	= 0.0;
		latitudeMin		= 0.0;
		longitudeMax	= 0.0;
	}
	AISZone(const AISZone& object)
	{
		latitudeMax		= object.latitudeMax;
		longitudeMin	= object.longitudeMin;
		latitudeMin		= object.latitudeMin;
		longitudeMax	= object.longitudeMax;
	}

	double latitudeMin;
	double longitudeMin;
	double latitudeMax;
	double longitudeMax;
} AISZone;

#endif // TCPAISZONEDEFINES_H
