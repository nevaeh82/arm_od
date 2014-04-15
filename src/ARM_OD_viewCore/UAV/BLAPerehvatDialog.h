#ifndef BLAPEREHVATDIALOG_H
#define BLAPEREHVATDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QLineEdit>

#include <QVector>

#include "Tabs/DbBla/DbUavManager.h"

#include "Map/IMapClient.h"

#include "Tabs/DbBla/DbUavManager.h"
#include "Tabs/DbBla/DbUavController.h"
#include "Db/Defines.h"

#include "Interfaces/SettingsManager/IDbBlaSettingsManager.h"
#include "SettingsManager/DbBlaSettingsManager.h"

namespace Ui {
class BLAPerehvatDialog;
}

class BLAPerehvatDialog : public QDialog
{
	Q_OBJECT
private:
	Ui::BLAPerehvatDialog *ui;

	int m_id;

	//IDbUavManager* m_dbUav;

	IMapClient* m_mapClient;

	IDbBlaSettingsManager* m_dbBlaSettingsManager;
	DbUavController* m_dbUavController;
	DbUavManager* m_dbUav;
public:
	explicit BLAPerehvatDialog(IMapClient*, QWidget *parent = NULL);
	~BLAPerehvatDialog();

	void init(int id, IDbUavManager* dbUav);

private:
	DBConnectionStruct getDbBlaConnectionSettings();

private slots:
	void treeItemChangedSlot(QTreeWidgetItem* item, int id);
};

#endif // BLAPEREHVATDIALOG_H
