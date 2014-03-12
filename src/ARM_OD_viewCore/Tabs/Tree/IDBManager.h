#ifndef IDBMANAGER_H
#define IDBMANAGER_H

#include <QMap>
#include <QVector>
class QVariant;
class QString;

class IDBManager
{
public:
	virtual ~IDBManager(){}

    virtual int set(int group, QMap<QString, QVariant>* data)               = 0;
    virtual int set_property(int group, QMap<QString, QVariant>* data)      = 0;

    virtual QVector<QMap<QString, QVariant> >* get(int id, int group)  = 0;
    virtual QVector<int> get(int group)                      = 0;

    virtual QMap<QString, QVariant>* get_bla_fields(int id)             = 0;
    virtual QMap<QString, QVariant>* get_bpla_fields(int id)             = 0;

    virtual void delete_bla(int id) = 0;
    virtual void delete_bpla(int id) = 0;
    virtual void delete_bla_property(int pid, int id) = 0;
    virtual void delete_bpla_property(int pid, int id) = 0;


    virtual void removeItem(int id, int group)               = 0;
};

#endif // IDBMANAGER_H
