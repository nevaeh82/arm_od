#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QThread>


#include "IDBManager.h"
#include "IDBController.h"
#include "DBController.h"

#include "ITreeModel.h"

//#include "DBCache.h"

#include "Common/CommandMessage.h"
#include "Tabs/ITabManager.h"

class DBManager : public QObject, public IDBManager
{
	Q_OBJECT
public:
	DBManager(ITabManager* tab_manager);
	~DBManager();

public:
	void set_model(ITreeModel *model/*_bla, ITreeModel* model_bpla*/);

public:
	virtual int set(int group, QMap<QString, QVariant>* data);
	virtual int set_property(int group, QMap<QString, QVariant>* data);

	virtual QVector<QMap<QString, QVariant> >* get(int id, int group);
	virtual QVector<int> get(int group);

	virtual void removeItem(int id, int group);

	virtual QMap<QString, QVariant>* get_bla_fields(int id);
	virtual QMap<QString, QVariant>* getBplaFields(int id);

	virtual void delete_bla(int id);
	virtual void delete_bpla(int id);
	virtual void delete_bla_property(int pid, int id);
	virtual void delete_bpla_property(int pid, int id);



private:
	ITabManager*        _tab_manager;
	DBController*       _db_controller;
	ITreeModel*         _model/*_bla*/;
//    ITreeModel*         _model_bpla;

//    DBCache             _db_cache;

signals:
	void signalSetDataDBController(QMap<QString, QVariant> *);
	void signalSetModel(ITreeModel* model);
	void signalSet(int group, QMap<QString, QVariant>* data);
	void signalSetPropertry(int group, QMap<QString, QVariant> *data);

private slots:
	void _slot_set_model(ITreeModel* model);
	void _slot_set(int group, QMap<QString, QVariant>* data);
	void _slot_set_property(int group, QMap<QString, QVariant> *data);

};

#endif // DBMANAGER_H
