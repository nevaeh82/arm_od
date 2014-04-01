#ifndef DBBLACONTROLLER_H
#define DBBLACONTROLLER_H

#include <QObject>
#include <QSqlDatabase>
#include "IDbBlaController.h"

class DbBlaController : public QObject, public IDbBlaController
{
	Q_OBJECT
private:
	QSqlDatabase m_db;
	QString     m_dbName;

public:
	explicit DbBlaController(QObject *parent = 0);
	~DbBlaController();

	bool addBla(const Bla&);
	bool addBla(const Bla&, const BlaInfo&);

	bool addBlaInfo(const BlaInfo&);

	bool addDevice(const Device&);

	bool addBlaMission(const BlaMission&);
	bool addTarget(const Target&);
signals:
	
public slots:
	
};

#endif // DBBLACONTROLLER_H
