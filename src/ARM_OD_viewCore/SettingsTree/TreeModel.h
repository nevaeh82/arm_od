#ifndef TreeModel_H
#define TreeModel_H

//#include <QtGui>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QStringList>
#include <QVariant>

class TreeItem;

#include "Interfaces/IDbChangedListener.h"

#include "Interfaces/IDbManager.h"

class TreeModel : public QAbstractItemModel, public IDbChangedListener
{
	Q_OBJECT

private:
	TreeItem*      m_rootItem;
	QStringList m_headersList;

	IDbManager* m_dbManager;

public:
	TreeModel(IDbManager* dbManager, const QStringList& headers, QObject *parent = 0);

	virtual ~TreeModel();

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

	QModelIndex parent(const QModelIndex &index) const;

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	void onSettingsNodeChanged(const SettingsNode & settingsNode);

	void onPropertyChanged(const Property& property);

	void onCleanSettings();

	void setStationsList(const QStringList& stationsList);

private:
	void refreshModel();

	void setupModelData(const QStringList &lines, TreeItem *parent);

	QString getTranslateItemNameFromReal(const QString& itemName) const;

signals:
	void onItemAddedSignal();




};

#endif // TreeModel_H
