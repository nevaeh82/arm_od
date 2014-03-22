#include "TcpAISZoneManager.h"

TcpAISZoneManager::TcpAISZoneManager(QObject* parent) :
	QObject(parent)
{
}

TcpAISZoneManager::~TcpAISZoneManager()
{
}

QString TcpAISZoneManager::getZone(const double& latitude1, const double& longitude1,
								   const double& latitude2, const double& longitude2)
{
	QString zone = TCP_AIS_URL_FULL_MAP;

	if (EUROPE_LATITUDE_MAX > latitude1 && latitude2 > EUROPE_LATITUDE_MIN) {
		if (EUROPE_LONGITUDE_MIN < longitude1 && longitude2 < EUROPE_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_EUROPE;
		}
	}
	if (NORTHAMERICA_LATITUDE_MAX > latitude1 && latitude2 > NORTHAMERICA_LATITUDE_MIN) {
		if (NORTHAMERICA_LONGITUDE_MIN < longitude1 && longitude2 < NORTHAMERICA_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_NORTHAMERICA;
		}
	}
	if (SOUTHAMERICA_LATITUDE_MAX > latitude1 && latitude2 > SOUTHAMERICA_LATITUDE_MIN) {
		if (SOUTHAMERICA_LONGITUDE_MIN < longitude1 && longitude2 < SOUTHAMERICA_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_SOUTHAMERICA;
		}
	}
	if (OCEANIA_LATITUDE_MAX > latitude1 && latitude2 > OCEANIA_LATITUDE_MIN) {
		if (OCEANIA_LONGITUDE_MIN < longitude1 && longitude2 < OCEANIA_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_OCEANIA;
		}
	}
	if (ASIA_LATITUDE_MAX > latitude1 && latitude2 > ASIA_LATITUDE_MIN) {
		if (ASIA_LONGITUDE_MIN < longitude1 && longitude2 < ASIA_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_ASIA;
		}
	}
	if (AFRICA_LATITUDE_MAX > latitude1 && latitude2 > AFRICA_LATITUDE_MIN) {
		if (AFRICA_LONGITUDE_MIN < longitude1 && longitude2 < AFRICA_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_AFRICA;
		}
	}
	if (ATLANTIC_LATITUDE_MAX > latitude1 && latitude2 > ATLANTIC_LATITUDE_MIN) {
		if (ATLANTIC_LONGITUDE_MIN < longitude1 && longitude2 < ATLANTIC_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_ATLANTIC;
		}
	}
	if (MALDIVES_LATITUDE_MAX > latitude1 && latitude2 > MALDIVES_LATITUDE_MIN) {
		if (MALDIVES_LONGITUDE_MIN < longitude1 && longitude2 < MALDIVES_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_MALDIVES;
		}
	}
	if (NORTHATLANTIC_LATITUDE_MAX > latitude1 && latitude2 > NORTHATLANTIC_LATITUDE_MIN) {
		if (NORTHATLANTIC_LONGITUDE_MIN < longitude1 && longitude2 < NORTHATLANTIC_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_NORTHATLANTIC;
		}
	}

	return zone;
}
