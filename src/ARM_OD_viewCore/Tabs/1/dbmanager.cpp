#include "dbmanager.h"
#include <QDebug>

DBManager::DBManager()
{
    _sdb = QSqlDatabase::addDatabase("QSQLITE");
}

DBManager::~DBManager()
{
    if(_sdb.isOpen())
        _sdb.close();
}

void DBManager::set_db(QString name)
{
    _sdb.setDatabaseName(name);
    if(!_sdb.open())
    {
        qDebug() << _sdb.lastError().text();
    }
}

void DBManager::set_table(QString name)
{
    _cur_table_name = name;
//    QString str;
//    str.append("CREATE TABLE ");
//    str.append(_cur_table_name);
//    str.append(" (number integer PRIMARY KEY NOT NULL, id integer);");
//    QSqlQuery query;
//    query.exec(str);
}

QList<QMap<QString, QVariant> > DBManager::get_data(QStringList list)
{
    QStringList req_fields("SELECT ");
    QStringList::const_iterator it;

    req_fields.append(list.join(", "));

/*    for(it = list.begin(); it != list.end(); ++it)
    {
        if((it != list.end()) && (it != list.begin()))
        {
            req_fields.append(", ");
        }
        req_fields.append(*it);

    }
    */
    req_fields.append(" FROM ");
    req_fields.append(_cur_table_name);
    QSqlQuery query;
    query.exec(req_fields.join(""));
    QSqlRecord rec = query.record();
    QList<QMap<QString, QVariant> >   list_result;
    while(query.next())
    {
        QMap<QString, QVariant> map;
        for(it = list.begin(); it != list.end(); ++it)
        {
            map.insert(*it, query.value(rec.indexOf(*it)));
        }
        list_result.push_back(map);
    }
    return list_result;
}

QList<QMap<QString, QVariant> > DBManager::get_data_cur_station(QStringList list, int pid)
{
    QString req_fields("SELECT ");
    QStringList::const_iterator it;
    for(it = list.begin(); it != list.end(); ++it)
    {
        if((it != list.end()) && (it != list.begin()))
        {
            req_fields.append(", ");
        }
        req_fields.append(*it);

    }
    req_fields.append(" FROM ");
    req_fields.append(_cur_table_name);
    req_fields.append(" WHERE pid=");
    req_fields.append(QString::number(pid));
//    req_fields.append("")
    QSqlQuery query;
    query.exec(req_fields);
    QSqlRecord rec = query.record();
    QList<QMap<QString, QVariant> >   list_result;
    while(query.next())
    {
        QMap<QString, QVariant> map;
        for(it = list.begin(); it != list.end(); ++it)
        {
            map.insert(*it, query.value(rec.indexOf(*it)));
        }
        list_result.push_back(map);
    }
    return list_result;
}

void DBManager::insert_data(int id)
{
    QString insert_string;
    insert_string.append("INSERT INTO ");
    insert_string.append(_cur_table_name);
    insert_string.append(" (id) VALUES (:id);");
    QSqlQuery query;
    query.prepare(insert_string);

    query.bindValue(":id", id);

    query.exec();
}

void DBManager::update(QString table_name, QString value_name, QString value, QString parent_name)
{
    QString update_string;
    update_string.append("UPDATE ");
    update_string.append(table_name);
    update_string.append(" SET \"");
    update_string.append("property");
    update_string.append("\"=");
    update_string.append(value);
    update_string.append(" WHERE ");

    set_table("Stations");


    QString req_fields("SELECT ");
    req_fields.append("id");
    req_fields.append(" FROM ");
    req_fields.append(_cur_table_name);
    req_fields.append(" WHERE name=\"");
    req_fields.append(parent_name);
    req_fields.append("\"");
    QSqlQuery query;
    bool err = query.exec(req_fields);
    QSqlRecord rec = query.record();
    QVariant pid = 0;
    while(query.next())
    {
        pid = query.value(rec.indexOf("id"));
    }

    update_string.append("pid=");
    update_string.append(pid.toString());
    update_string.append(" AND name=\"");
    update_string.append(value_name);
    update_string.append("\"");

    set_table(table_name);
    err = query.exec(update_string);
    qDebug() << err;
}
