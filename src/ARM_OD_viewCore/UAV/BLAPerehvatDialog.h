#ifndef BLAPEREHVATDIALOG_H
#define BLAPEREHVATDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../Tabs/Tree/DBManager.h"

#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QLineEdit>

#include <QVector>

#include "../MapInterface/IMapClient.h"

#include "Tabs/DbBla/DbBlaManager.h"

namespace Ui {
class BLAPerehvatDialog;
}

class BLAPerehvatDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit BLAPerehvatDialog(IMapClient* map_client);
    ~BLAPerehvatDialog();

	void init(int id, IDbBlaManager* db_bla, DBManager* db_evil);
    
private:
    Ui::BLAPerehvatDialog *ui;

    int                 _id;

	IDbBlaManager*          _db_bla;
    DBManager*          _db_evil;

    IMapClient*     _map_client;

private slots:
    void treeItemChangedSlot(QTreeWidgetItem* item, int id);
};

#endif // BLAPEREHVATDIALOG_H
