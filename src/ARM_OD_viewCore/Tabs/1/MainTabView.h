#ifndef MAINTABVIEW_H
#define MAINTABVIEW_H

//#include <QAction>
#include <QWidget>
#include <QMap>

#include "BLAWidgetDelegate.h"
#include "MainTreeModelManager.h"

class MainTabView : public QWidget
{
    Q_OBJECT
public:
    MainTabView(QWidget *parent = 0);
    ~MainTabView() {}

private:
    QTreeView               *_tree_view;
    BLAWidgetDelegate       *_delegate;

    MainTreeModelManager    *_model;

public:
    QTreeView *get_tree();

    void setModel(MainTreeModelManager *model);

//    void insertChild(QString value, QString parent_name);
//    void insertRootChild(QString name, int role);
//    void insertRootChild2Column(QString parent_name, QString name, int role);

//     void insertRowSettingProperties(QString parent_name, QString ID, QString property);

//    //void insertRootChildren(QString name);
//    bool insertColumn(const QModelIndex &parent);
//    void insertRow();
//    bool removeColumn(const QModelIndex &parent);
//    void removeRow();
//    void updateActions();

    void resizeEvent(QResizeEvent *event);


//    void insertRowProperties(QString parent_name, QString ID, QString property);
//    void updateItem(QModelIndex start_index, QString value, QString name);

//    bool itemPresent(QString id, QString name, QModelIndex &index);

//    void updateState(QString name, bool state);

signals:
    void signalResize(int width, int height);

//protected:
//    bool eventFilter(QObject *, QEvent *);
};

#endif // MAINTABVIEW_H
