#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <QObject>
#include <QtSql>
#include <QString>
#include <QStringList>

#include <QMap>
#include <QVariant>

#include "IDBController.h"

#include "Logger.h"

class DBController : public QObject, public IDBController
{
    Q_OBJECT
public:
    DBController();
    ~DBController();

public:
//    virtual QMap<QString, QVariant>* get_data(int id, int pid);
//    virtual QMap<QString, QVariant>* get_data(int pid);

//public slots:
//    virtual void set_data(QMap<QString, QVariant>* data);

    virtual QVector<int> get_list_bla();
    virtual QVector<int> get_list_bpla();

    virtual QVector<QMap<QString, QVariant> >* get_property_bla(int pid);
    virtual QVector<QMap<QString, QVariant> >* get_property_bpla(int pid);

    virtual QMap<QString, QVariant>* get_bla_fields(int id);
    virtual QMap<QString, QVariant>* get_bpla_fields(int id);



    virtual void set_bla(QMap<QString, QVariant>* data);
    virtual void set_bla_property(QMap<QString, QVariant>* data);

    virtual void set_bpla(QMap<QString, QVariant>* data);
    virtual void set_bpla_property(QMap<QString, QVariant>* data);

    virtual void delete_bla(int id);
    virtual void delete_bpla(int id);
    virtual void delete_bla_property(int pid, int id);
    virtual void delete_bpla_property(int pid, int id);


//private slots:
//    void _slot_set(QMap<QString, QVariant>* data);

private:
    QSqlDatabase _sdb;
    QString     _db_name;

private:
     int _read_settings(QString path_to_ini_file);
     int _set_db();
     void _set_table(QString name);

signals:
     void signalFinished();
//     void signalSet(QMap<QString, QVariant> *);

};

#endif // DBCONTROLLER_H
