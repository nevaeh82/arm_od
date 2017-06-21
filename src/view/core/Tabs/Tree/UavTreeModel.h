#ifndef BLATREEMODEL_H
#define BLATREEMODEL_H

#include <QObject>
#include <QTimer>
#include <TreeModel/TreeModelBase.h>

#include "Interfaces/IUavHistoryListener.h"

class UavTreeModel : public TreeModelBase, public IUavHistoryListener
{
	Q_OBJECT

private:
	QTimer m_treeUpdater;
	bool m_isNeedRedraw;

	QString m_targetRole;
	int m_isExtraInfo;

public:
	explicit UavTreeModel(const QStringList& headers, QObject *parent = 0);
	virtual ~UavTreeModel();

	QVariant data(const QModelIndex &index, int role) const;

protected:
	virtual QString getTranslateItemNameFromReal(const QString &itemName) const;
	virtual bool updateModelData(TreeItem *item);

public:
	virtual void onUavAdded(const Uav &uav, const QString& uavRole);
	virtual void onUavRemoved(const Uav &uav, const QString& uavRole);
	virtual void onUavInfoChanged(const UavInfo &uavInfo, const QString& uavRole,
								  const QVector<QPointF> &tail = QVector<QPointF>(),
								  const QVector<QPointF> &tailStdDev = QVector<QPointF>());
	virtual void onStatusChanged(Status) {}

	void setTargetRole(const QString& role);

private:
	void onPropertyChanged(const UavInfo &uavInfo, const uint propId, const QString& name, const QVariant& value);

	void addSourceNode(TreeItem* item, uint sourceType, QString name, uint uavId);

	int getId(const UavInfo &uav);
	int getId(const Uav &uav);

private slots:
	void updateData();
	void onExtraBoardInfo(int);
};

#endif // BLATREEMODEL_H
