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
    virtual int setProperty(int group, QMap<QString, QVariant>* data)      = 0;

    virtual QVector<QMap<QString, QVariant> >* get(int id, int group)  = 0;
    virtual QVector<int> get(int group)                      = 0;

    virtual QMap<QString, QVariant>* getFriendBplaFields(int id) = 0;
    virtual QMap<QString, QVariant>* getEnemyBplaFields(int id) = 0;

    virtual void deleteFriendBpla(int id) = 0;
    virtual void deleteEnemyBpla(int id) = 0;
    virtual void deleteFriendBplaProperty(int pid, int id) = 0;
    virtual void deleteEnemyBplaProperty(int pid, int id) = 0;


    virtual void removeItem(int id, int group)               = 0;
};

#endif // IDBMANAGER_H
