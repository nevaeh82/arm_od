#ifndef BLATREEMODEL_H
#define BLATREEMODEL_H

#include <QObject>
#include <TreeModel/TreeModelBase.h>

#include "Interfaces/IUavDbChangedListener.h"

class UavTreeModel : public TreeModelBase, public IUavDbChangedListener
{
	Q_OBJECT
public:
	explicit UavTreeModel(const QStringList& headers, QObject *parent = 0);
	virtual ~UavTreeModel();

protected:
	virtual QString getTranslateItemNameFromReal(const QString &itemName) const;
	virtual bool updateModelData(TreeItem *item);

public:
	virtual void onUavAdded(const Uav &uav, const QString& uavRole);
	virtual void onUavRemoved(const Uav &uav, const QString& uavRole);
	virtual void onUavInfoChanged(const UavInfo &uavInfo, const QString& uavRole);

private:

	void onPropertyChanged(const UavInfo &uavInfo, const uint propId, const QString& name, const QVariant& value);
};

#endif // BLATREEMODEL_H
