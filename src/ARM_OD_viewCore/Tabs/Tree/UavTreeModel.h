#ifndef BLATREEMODEL_H
#define BLATREEMODEL_H

#include <QObject>
#include <QTimer>
#include <TreeModel/TreeModelBase.h>

#include "Interfaces/IUavDbChangedListener.h"

class UavTreeModel : public TreeModelBase, public IUavDbChangedListener
{
	Q_OBJECT

private:
	QTimer m_treeUpdater;
	bool m_isNeedRedraw;

public:
	explicit UavTreeModel(const QStringList& headers, QObject *parent = 0);
	virtual ~UavTreeModel();

	QVariant data(const QModelIndex &index, int role) const;

protected:
	virtual QString getTranslateItemNameFromReal(const QString &itemName) const;
	virtual bool updateModelData(TreeItem *item);

public:
	virtual void onUavAdded(const Uav &uav);
	virtual void onUavRemoved(const Uav &uav);
	virtual void onUavInfoChanged(const UavInfo &uavInfo);

private:
	void onPropertyChanged(const UavInfo &uavInfo, const uint propId, const QString& name, const QVariant& value);

private slots:
	void updateData();
};

#endif // BLATREEMODEL_H
