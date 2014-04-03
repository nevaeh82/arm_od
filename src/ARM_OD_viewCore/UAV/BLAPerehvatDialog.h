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

namespace Ui {
class BLAPerehvatDialog;
}

class BLAPerehvatDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit BLAPerehvatDialog(IMapClient* map_client);
    ~BLAPerehvatDialog();

    void init(int id, DBManager* db_bla, DBManager* db_evil);
    
private:
    Ui::BLAPerehvatDialog *ui;

    int                 _id;

    DBManager*          m_friendBplaDb;
    DBManager*          m_enemyBplaDbl;

    IMapClient*     m_mapClient;

private slots:
    void treeItemChangedSlot(QTreeWidgetItem* item, int id);
};

#endif // BLAPEREHVATDIALOG_H
