#include "TcpAISZoneManager.h"

TcpAISZoneManager::TcpAISZoneManager(QObject* parent) :
	QObject(parent)
{
}

TcpAISZoneManager::~TcpAISZoneManager()
{
}


AISZone TcpAISZoneManager::getLastZone()
{
	return m_lastZone;
}

QString TcpAISZoneManager::getZone(const AISZone& aisZone)
{
	m_lastZone = aisZone;

	QString zone = TCP_AIS_URL_FULL_MAP;

	if (EUROPE_LATITUDE_MAX > m_lastZone.latitudeMax
			&& m_lastZone.latitudeMin > EUROPE_LATITUDE_MIN) {
		if (EUROPE_LONGITUDE_MIN < m_lastZone.longitudeMin
				&& m_lastZone.longitudeMax < EUROPE_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_EUROPE;
		}
	}
	if (NORTHAMERICA_LATITUDE_MAX > m_lastZone.latitudeMax
			&& m_lastZone.latitudeMin > NORTHAMERICA_LATITUDE_MIN) {
		if (NORTHAMERICA_LONGITUDE_MIN < m_lastZone.longitudeMin
				&& m_lastZone.longitudeMax < NORTHAMERICA_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_NORTHAMERICA;
		}
	}
	if (SOUTHAMERICA_LATITUDE_MAX > m_lastZone.latitudeMax
			&& m_lastZone.latitudeMin > SOUTHAMERICA_LATITUDE_MIN) {
		if (SOUTHAMERICA_LONGITUDE_MIN < m_lastZone.longitudeMin
				&& m_lastZone.longitudeMax < SOUTHAMERICA_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_SOUTHAMERICA;
		}
	}
	if (OCEANIA_LATITUDE_MAX > m_lastZone.latitudeMax
			&& m_lastZone.latitudeMin > OCEANIA_LATITUDE_MIN) {
		if (OCEANIA_LONGITUDE_MIN < m_lastZone.longitudeMin
				&& m_lastZone.longitudeMax < OCEANIA_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_OCEANIA;
		}
	}
	if (ASIA_LATITUDE_MAX > m_lastZone.latitudeMax
			&& m_lastZone.latitudeMin > ASIA_LATITUDE_MIN) {
		if (ASIA_LONGITUDE_MIN < m_lastZone.longitudeMin
				&& m_lastZone.longitudeMax < ASIA_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_ASIA;
		}
	}
	if (AFRICA_LATITUDE_MAX > m_lastZone.latitudeMax
			&& m_lastZone.latitudeMin > AFRICA_LATITUDE_MIN) {
		if (AFRICA_LONGITUDE_MIN < m_lastZone.longitudeMin
				&& m_lastZone.longitudeMax < AFRICA_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_AFRICA;
		}
	}
	if (ATLANTIC_LATITUDE_MAX > m_lastZone.latitudeMax
			&& m_lastZone.latitudeMin > ATLANTIC_LATITUDE_MIN) {
		if (ATLANTIC_LONGITUDE_MIN < m_lastZone.longitudeMin
				&& m_lastZone.longitudeMax < ATLANTIC_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_ATLANTIC;
		}
	}
	if (MALDIVES_LATITUDE_MAX > m_lastZone.latitudeMax
			&& m_lastZone.latitudeMin > MALDIVES_LATITUDE_MIN) {
		if (MALDIVES_LONGITUDE_MIN < m_lastZone.longitudeMin
				&& m_lastZone.longitudeMax < MALDIVES_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_MALDIVES;
		}
	}
	if (NORTHATLANTIC_LATITUDE_MAX > m_lastZone.latitudeMax
			&& m_lastZone.latitudeMin > NORTHATLANTIC_LATITUDE_MIN) {
		if (NORTHATLANTIC_LONGITUDE_MIN < m_lastZone.longitudeMin
				&& m_lastZone.longitudeMax < NORTHATLANTIC_LONGITUDE_MAX) {
			zone = TCP_AIS_URL_NORTHATLANTIC;
		}
	}

	return zone;
}
