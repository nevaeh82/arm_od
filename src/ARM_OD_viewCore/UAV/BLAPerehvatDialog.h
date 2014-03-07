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

    DBManager*          _db_bla;
    DBManager*          _db_evil;

    QComboBox*      _cb_bla;
    QComboBox*      _cb_evil;

    QTreeView*      _tr_bla;
    QTreeView*      _tr_evil;

    QTreeWidget*    _tr_bpla;

    IMapClient*     _map_client;

private slots:
    void _slot_item_changed(QTreeWidgetItem* item, int id);
};

#endif // BLAPEREHVATDIALOG_H
