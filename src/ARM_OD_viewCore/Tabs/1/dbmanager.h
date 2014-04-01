#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql>
#include <QString>
#include <QStringList>

class DBManager
{
public:
    DBManager();
    ~DBManager();

public:
    void set_db(QString name);
    void set_table(QString name);
    QList<QMap<QString, QVariant> > get_data(QStringList list);
    QList<QMap<QString, QVariant> > get_data_cur_station(QStringList list, int pid);
    void insert_data(int id);
    void update(QString table_name, QString value_name, QString value, QString parent_name);

private:
    QSqlDatabase _sdb;
    QString      _cur_table_name;

};

#endif // DBMANAGER_H
