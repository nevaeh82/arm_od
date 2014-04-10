#include "BLAPerehvatDialog.h"
#include "ui_blaperehvatdialog.h"

BLAPerehvatDialog::BLAPerehvatDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::BLAPerehvatDialog)
{
	ui->setupUi(this);

	m_id = 0;
	m_dbBla = NULL;
	m_dbEvil = NULL;
}

BLAPerehvatDialog::~BLAPerehvatDialog()
{
	delete ui;
}

void BLAPerehvatDialog::init(int id, IDbUavManager *db_bla, DBManager *db_evil)
{
	m_id = id;
	m_dbBla = db_bla;

	QList<Uav> enemyUavs;

	m_dbBla->getUavsByRole(ENEMY_UAV_ROLE, enemyUavs);
	ui->blaLE->setText(tr("UAV #") + QString::number(m_id));

	QList<QTreeWidgetItem *> items;

	foreach (Uav enemyUav, enemyUavs){
		QTreeWidgetItem *it = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(tr("UAV_: #%1")).arg(enemyUav.id)));
	//	QMap<QString, QVariant>* res_evil_fields = m_dbEvil->get_bpla_fields(res_evil.at(i));

		/*if(res_evil_fields->value("state").toInt() == 0) {
			it->setCheckState(0, Qt::Unchecked);
		}
		else {
			it->setCheckState(0, Qt::Checked);
		}
*/
		items.append(it);
	}
	ui->bplaTreeWidget->insertTopLevelItems(0, items);


	connect(ui->bplaTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(treeItemChangedSlot(QTreeWidgetItem*, int)));


	/*m_id = id;
	m_dbBla = db_bla;
	m_dbEvil = db_evil;

	QVector<int> res_evil = m_dbEvil->get(1);

	ui->blaLE->setText(tr("UAV #") + QString::number(m_id));

	QList<QTreeWidgetItem *> items;
	for (int i = 0; i < res_evil.size(); ++i) {
		QTreeWidgetItem *it = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(tr("UAV_: #%1")).arg(res_evil.at(i))));
		QMap<QString, QVariant>* res_evil_fields = m_dbEvil->get_bpla_fields(res_evil.at(i));

		if(res_evil_fields->value("state").toInt() == 0) {
			it->setCheckState(0, Qt::Unchecked);
		}
		else {
			it->setCheckState(0, Qt::Checked);
		}

		items.append(it);
	}
	ui->bplaTreeWidget->insertTopLevelItems(0, items);

	connect(ui->bplaTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(treeItemChangedSlot(QTreeWidgetItem*, int)));*/
}

void BLAPerehvatDialog::treeItemChangedSlot(QTreeWidgetItem *item, int id)
{
	/*TODO: FINISH REFACTORING IDbBlaManager */
	/*qDebug() << item->text(0) << id << item->checkState(0);
	QStringList ls;
	ls = item->text(0).split(tr("#"));

	int id_bpla = ls.at(1).toInt();
	QMap<QString, QVariant>* db_bpla = m_dbEvil->get_bpla_fields(id_bpla);
	QString ss = "";
	if(item->checkState(0) == 0)
	{
		db_bpla->insert("state", 0);
		QVector<QMap<QString, QVariant> >* g = m_dbBla->get(m_id, 0);
		int id_in_tree = 0;
		for(int i = 0; i < g->size(); ++i) {
			if(g->at(i).value("value").toInt() == id_bpla) {
				id_in_tree = g->at(i).value("id").toInt();
			}
		}

		if(id_in_tree != 0) {
			m_dbBla->delete_bla_property(m_id, id_in_tree);
			m_mapClient->remove_perehvat(m_id, id_in_tree);
		}
	}
	else {
		db_bpla->insert("state", 1);

		QVector<QMap<QString, QVariant> >* g = m_dbBla->get(m_id, 0);
		QString s = g->at(0).value("value").toString();
		if(ss != "")
		{
			ss += ", ";
		}

		ss += QString::number(id_bpla);

		QMap<QString, QVariant>* data = new QMap<QString, QVariant>;
		data->insert("pid", QVariant::fromValue(m_id));
		data->insert("name", QVariant::fromValue(tr("Target #")));

		data->insert("value", QVariant::fromValue(ss));
		data->insert("state", QVariant::fromValue(1));

		m_dbBla->set_property(0, data);
		m_mapClient->add_perehvat(m_id, id_bpla);
	}

	m_dbEvil->set(1, db_bpla);*/
}
