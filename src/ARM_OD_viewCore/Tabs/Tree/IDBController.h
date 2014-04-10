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

    virtual QVector<int> getFriendBplaList()                  = 0;
    virtual QVector<int> getEnemyBplaList()                 = 0;

    virtual QVector<QMap<QString, QVariant> >* getFriendBplaProperty(int pid)       = 0;
    virtual QVector<QMap<QString, QVariant> >* getEnemyBplaPropert(int pid)      = 0;

    virtual QMap<QString, QVariant>* getFriendBplaFields(int id)          = 0 ;
    virtual QMap<QString, QVariant>* getEnemyBplaFields(int id)            = 0;


    virtual void setFriendBpla(QMap<QString, QVariant>* data)              = 0;
    virtual void setFriendBplaProperty(QMap<QString, QVariant>* data)     = 0;

    virtual void setEnemyBpla(QMap<QString, QVariant>* data)             = 0;
    virtual void setEnemyBplaProperty(QMap<QString, QVariant>* data)    = 0;

    virtual void deleteFriendBpla(int id) = 0;
    virtual void deleteEnemyBpla(int id) = 0;
    virtual void deleteFriendBplaProperty(int pid, int id) = 0;
    virtual void deleteEnemyBplaProperty(int pid, int id) = 0;





//    virtual QList<QMap<QString, QVariant> > get_all_data_table(int pid)     = 0;

};

#endif // IDBCONTROLLER_H
