#ifndef IDBCONTROLLER_H
#define IDBCONTROLLER_H

#include <QMap>
#include <QVector>
class QString;
class QVariant;

class IDBController
{
public:
	virtual ~IDBController(){}

    virtual QVector<int> get_list_bla()                  = 0;
    virtual QVector<int> get_list_bpla()                 = 0;

    virtual QVector<QMap<QString, QVariant> >* get_property_bla(int pid)       = 0;
    virtual QVector<QMap<QString, QVariant> >* get_property_bpla(int pid)      = 0;

    virtual QMap<QString, QVariant>* get_bla_fields(int id)          = 0 ;
    virtual QMap<QString, QVariant>* get_bpla_fields(int id)            = 0;


    virtual void set_bla(QMap<QString, QVariant>* data)              = 0;
    virtual void set_bla_property(QMap<QString, QVariant>* data)     = 0;

    virtual void set_bpla(QMap<QString, QVariant>* data)             = 0;
    virtual void set_bpla_property(QMap<QString, QVariant>* data)    = 0;

    virtual void delete_bla(int id) = 0;
    virtual void delete_bpla(int id) = 0;
    virtual void delete_bla_property(int pid, int id) = 0;
    virtual void delete_bpla_property(int pid, int id) = 0;





//    virtual QList<QMap<QString, QVariant> > get_all_data_table(int pid)     = 0;

};

#endif // IDBCONTROLLER_H
