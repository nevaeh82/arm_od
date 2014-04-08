#ifndef BLATREEMODEL_H
#define BLATREEMODEL_H

#include <QObject>
#include <TreeModel/TreeModelBase.h>

#include "Interfaces/IBlaDbChangedListener.h"

class UavTreeModel : public TreeModelBase, public IBlaDbChangedListener
{
	Q_OBJECT
public:
	explicit UavTreeModel(const QStringList& headers, QObject *parent = 0);
	virtual ~UavTreeModel();

protected:
	virtual QString getTranslateItemNameFromReal(const QString &itemName) const;
	virtual bool updateModelData(TreeItem *item);

public:
	virtual void onBlaAdded(const Bla &bla);
	virtual void onBlaRemoved(const Bla &bla);
	virtual void onBlaInfoChanged(const BlaInfo &blaInfo);

private:

	void onPropertyChanged(const BlaInfo &blaInfo, const uint propId, const QString& name, const QVariant& value);
};

#endif // BLATREEMODEL_H
