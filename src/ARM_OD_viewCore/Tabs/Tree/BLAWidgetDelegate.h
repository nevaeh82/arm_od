#ifndef BLAWIDGETDELEGATE_H
#define BLAWIDGETDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QPainter>

class BLAWidgetDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:

    BLAWidgetDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    QPixmap *_pm_round_red;
    QPixmap *_pm_round_green;
    QString _styleSheet;

    QVector<QString> _cb_items_filter;

    int     _cur_view;
    
signals:
    
public slots:
    
};

#endif // BLAWIDGETDELEGATE_H
