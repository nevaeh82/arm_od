#include "BLAPerehvatDialog.h"
#include "ui_blaperehvatdialog.h"

BLAPerehvatDialog::BLAPerehvatDialog(IMapClient* map_client) :
    QDialog(0),
    ui(new Ui::BLAPerehvatDialog)
{
    ui->setupUi(this);

    _map_client = map_client;

}

BLAPerehvatDialog::~BLAPerehvatDialog()
{
    delete _cb_bla;
    delete ui;
}

void BLAPerehvatDialog::init(int id, DBManager *db_bla, DBManager *db_evil)
{
    _id = id;
    _db_bla = db_bla;
    _db_evil = db_evil;

    QVector<int> res_evil = _db_evil->get(1);



//    _cb_bla = new QComboBox();
//    QVector<int> res  = _db_bla->get(0);
//    for(int i = 0; i < res.size(); i++)
//    {
//        _cb_bla->addItem(QString::number(res.at(i)));
//    }

    QVBoxLayout* vb = new QVBoxLayout();


    QLineEdit* le_bla = new QLineEdit();
	le_bla->setText(tr("UAV #") + QString::number(_id));
	/// UAV - Unmanned aerial vehicle

    QHBoxLayout* hb = new QHBoxLayout();
    vb->addWidget(le_bla, Qt::AlignTop);

     _tr_bpla = new QTreeWidget();
     _tr_bpla->setColumnCount(1);
     QList<QTreeWidgetItem *> items;
     for (int i = 0; i < res_evil.size(); ++i)
     {
		 QTreeWidgetItem *it = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(tr("UAV_: #%1")).arg(res_evil.at(i))));
         QMap<QString, QVariant>* res_evil_fields = _db_evil->get_bpla_fields(res_evil.at(i));


         if(res_evil_fields->value("state").toInt() == 0)
         {
             it->setCheckState(0, Qt::Unchecked);
         }
         else
         {
             it->setCheckState(0, Qt::Checked);
         }

         items.append(it);
     }
     _tr_bpla->insertTopLevelItems(0, items);

     connect(_tr_bpla, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(_slot_item_changed(QTreeWidgetItem*, int)));

//    vb->addLayout(hb);
    vb->addWidget(_tr_bpla);


    this->setLayout(vb);

}

void BLAPerehvatDialog::_slot_item_changed(QTreeWidgetItem *item, int id)
{
    qDebug() << item->text(0) << id << item->checkState(0);
    QStringList ls;
	ls = item->text(0).split(tr("#"));

    int id_bpla = ls.at(1).toInt();
    QMap<QString, QVariant>* db_bpla = _db_evil->get_bpla_fields(id_bpla);
    QString ss = "";
    if(item->checkState(0) == 0)
    {
        db_bpla->insert("state", 0);
//        _db_bla->removeItem(_id, 0);
        QVector<QMap<QString, QVariant> >* g = _db_bla->get(_id, 0);
//        QString s = g->at(0).value("value").toString();
//        QStringList sl = s.split(", ");
//        QStringList sl1;
        int id_in_tree = 0;
        for(int i = 0; i < g->size(); ++i)
        {
            if(g->at(i).value("value").toInt() == id_bpla)
            {
                id_in_tree = g->at(i).value("id").toInt();
            }
        }
//        ss = sl1.join(",");

        if(id_in_tree != 0)
        {
            _db_bla->delete_bla_property(_id, id_in_tree);
            _map_client->remove_perehvat(_id, id_in_tree);
        }

//        qDebug() << g->value("value");
//        return;
    }
    else
    {
        db_bpla->insert("state", 1);

        QVector<QMap<QString, QVariant> >* g = _db_bla->get(_id, 0);
        QString s = g->at(0).value("value").toString();
        if(ss != "")
        {
            ss += ", ";
        }

        ss += QString::number(id_bpla);


        QMap<QString, QVariant>* data = new QMap<QString, QVariant>;
        data->insert("pid", QVariant::fromValue(_id));
    //    data->insert("id", QVariant::fromValue(0));
		data->insert("name", QVariant::fromValue(tr("Target #")));

        data->insert("value", QVariant::fromValue(ss));
        data->insert("state", QVariant::fromValue(1));

        _db_bla->set_property(0, data);
        _map_client->add_perehvat(_id, id_bpla);
    }



    _db_evil->set(1, db_bpla);
    //_db_bla->get()

}
