#include "MainTabBar.h"

#include <QDebug>

MainTabBar::MainTabBar(QWidget *parent)
    : QTabBar(parent)
{
    _pm_round_red = new QPixmap(":/images/signals/bullet_red.png");
    _pm_round_green = new QPixmap(":/images/signals/bullet_green.png");

    this->addTab(QString("Гал"));
    this->addTab(QString("Пункт 1"));
    this->addTab(QString("Пункт 2"));
    qDebug() << this->children().size();

    QLabel *lbl0 = new QLabel();
    lbl0->setFixedSize(16, 16);
    lbl0->setPixmap(_pm_round_red->scaled(16,16,Qt::KeepAspectRatio));
    this->setTabButton(0, QTabBar::LeftSide, lbl0);

    QLabel *lbl1 = new QLabel();
    lbl1->setFixedSize(16, 16);
    lbl1->setPixmap(_pm_round_red->scaled(16,16,Qt::KeepAspectRatio));
    this->setTabButton(1, QTabBar::LeftSide, lbl1);

    QLabel *lbl2 = new QLabel();
    lbl2->setFixedSize(16, 16);
    lbl2->setPixmap(_pm_round_red->scaled(16,16,Qt::KeepAspectRatio));
    this->setTabButton(2, QTabBar::LeftSide, lbl2);

    //lbl0->setPixmap(_pm_round_green->scaled(16,16,Qt::KeepAspectRatio));

    _setIndicate(false);
}

void MainTabBar::_setIndicate(bool state)
{
    QLabel *lbl;
    QWidget *pWidget = this->tabButton(1, QTabBar::LeftSide);//->setTabData(1, /*->tabData(1);*/
    if(pWidget->metaObject()->className() == "QLabel")
    {
        lbl = (QLabel *)pWidget;
    }
    else
    {
        QList<QLabel *> allElements = pWidget->findChildren<QLabel *>();
        if(allElements.count() != 1)
        {
            //qDebug() << "Error";
            return;
        }
    }
    if(state == false)
    {
        lbl->setPixmap(_pm_round_green->scaled(16,16,Qt::KeepAspectRatio));
    }
}
