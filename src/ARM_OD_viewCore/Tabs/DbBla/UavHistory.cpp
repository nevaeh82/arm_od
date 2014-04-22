#include "UavHistory.h"

UavHistory::UavHistory(QSqlDatabase database, QObject *parent)
	: QObject(parent)
	, m_database(database)
{
}

UavHistory::~UavHistory()
{
	stop();
}

bool UavHistory::dateExists(const QDateTime& date)
{
	return false;
}

bool UavHistory::start(const QDateTime& start, const QDateTime& end)
{
	return false;
}

void UavHistory::stop()
{

}

void UavHistory::sendNextSecond()
{

}
