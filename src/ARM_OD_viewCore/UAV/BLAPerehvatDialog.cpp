﻿#include "BLAPerehvatDialog.h"
#include "ui_blaperehvatdialog.h"

BLAPerehvatDialog::BLAPerehvatDialog(IMapClient* map_client) :
	QDialog(0),
	ui(new Ui::BLAPerehvatDialog)
{
	ui->setupUi(this);

	m_mapClient = map_client;

}

BLAPerehvatDialog::~BLAPerehvatDialog()
{
	delete ui;
}

void BLAPerehvatDialog::init(int id, DBManager *db_bla, DBManager *db_evil)
{
	_id = id;
	m_friendBplaDb = db_bla;
	m_enemyBplaDbl = db_evil;

	QVector<int> res_evil = m_enemyBplaDbl->get(1);

	ui->blaLE->setText(tr("UAV #") + QString::number(_id));

	QList<QTreeWidgetItem *> items;
	for (int i = 0; i < res_evil.size(); ++i) {
		QTreeWidgetItem *it = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(tr("UAV_: #%1")).arg(res_evil.at(i))));
		QMap<QString, QVariant>* res_evil_fields = m_enemyBplaDbl->getBplaFields(res_evil.at(i));


		if(res_evil_fields->value("state").toInt() == 0) {
			it->setCheckState(0, Qt::Unchecked);
		}
		else {
			it->setCheckState(0, Qt::Checked);
		}

		items.append(it);
	}
	ui->bplaTreeWidget->insertTopLevelItems(0, items);

	connect(ui->bplaTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(treeItemChangedSlot(QTreeWidgetItem*, int)));
}

void BLAPerehvatDialog::treeItemChangedSlot(QTreeWidgetItem *item, int id)
{
	qDebug() << item->text(0) << id << item->checkState(0);
	QStringList ls;
	ls = item->text(0).split(tr("#"));

	int id_bpla = ls.at(1).toInt();
	QMap<QString, QVariant>* enemyBplaFields = m_enemyBplaDbl->getBplaFields(id_bpla);
	QString ss = "";
	if(item->checkState(0) == 0)
	{
		enemyBplaFields->insert("state", 0);
		QVector<QMap<QString, QVariant> >* g = m_friendBplaDb->get(_id, 0);
		int id_in_tree = 0;
		for(int i = 0; i < g->size(); ++i) {
			if(g->at(i).value("value").toInt() == id_bpla) {
				id_in_tree = g->at(i).value("id").toInt();
			}
		}

		if(id_in_tree != 0) {
			m_friendBplaDb->delete_bla_property(_id, id_in_tree);
			m_mapClient->removeInterception(_id, id_in_tree);
		}
	} else {
		enemyBplaFields->insert("state", 1);

		QVector<QMap<QString, QVariant> >* g = m_friendBplaDb->get(_id, 0);
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

		m_friendBplaDb->set_property(0, data);
		m_mapClient->addInterception(_id, id_bpla);
	}
}
