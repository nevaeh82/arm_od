#ifndef BLAPEREHVATDIALOG_H
#define BLAPEREHVATDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Tabs/Tree/DBManager.h"

#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QLineEdit>

#include <QVector>

#include "Map/IMapClient.h"

#include "Tabs/DbBla/DbUavManager.h"

namespace Ui {
class BLAPerehvatDialog;
}

class BLAPerehvatDialog : public QDialog
{
	Q_OBJECT
private:
	Ui::BLAPerehvatDialog *ui;

	int m_id;

	IDbUavManager* m_friendBplaDb;
	DBManager* m_enemyBplaDb;

	IMapClient* m_mapClient;

public:
	explicit BLAPerehvatDialog(IMapClient* map_client);
	~BLAPerehvatDialog();

	void init(int id, IDbUavManager* friendBplaDb, DBManager* enemyBplaDb);

private slots:
	void treeItemChangedSlot(QTreeWidgetItem* item, int id);
};

#endif // BLAPEREHVATDIALOG_H
