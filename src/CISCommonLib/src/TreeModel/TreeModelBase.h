#ifndef TREEMODELBASE_H
#define TREEMODELBASE_H


#include <QAbstractItemModel>
#include <QModelIndex>
#include <QStringList>
#include <QVariant>

class TreeItem;

class TreeModelBase : public QAbstractItemModel
{
	Q_OBJECT

protected:
	TreeItem*      m_rootItem;
	QStringList m_headersList;

public:
	TreeModelBase(const QStringList& headers, QObject *parent = 0);

	virtual ~TreeModelBase();

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

	QModelIndex parent(const QModelIndex &index) const;

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());


protected:
	virtual QString getTranslateItemNameFromReal(const QString& itemName) const;
	virtual void refreshModel();
	virtual bool updateModelData(TreeItem *item);

signals:
	void onItemAddedSignal();

};

#endif // TREEMODELBASE_H
