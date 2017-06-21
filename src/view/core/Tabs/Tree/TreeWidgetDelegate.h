#ifndef TREEWIDGETDELEGATE_H
#define TREEWIDGETDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QPainter>

class TreeWidgetDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TreeWidgetDelegate(QObject *parent = 0);

	//void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QPixmap *_pm_round_red;
    QPixmap *_pm_round_green;
    QString _styleSheet;

    
signals:
    
public slots:
    
};

#endif // TREEWIDGETDELEGATE_H
