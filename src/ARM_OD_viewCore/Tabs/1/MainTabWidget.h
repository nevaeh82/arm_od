#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include <QTabWidget>
#include <QVBoxLayout>

#include <QPushButton>

#include <QMap>
#include "MainTab.h"

//#include "../TCP/TCPService.h"

#include "../Graphics/GraphicWidget.h"
#include "TabsProperty.h"

#include "../DBForms/IDBView.h"

#include "Controller.h"

#include "IMainTabWidget.h"

class MainTabWidget : public QTabWidget, /*TCPService,*/ public IMainTabWidget
{
    Q_OBJECT
public:
    MainTabWidget(QWidget *parent, QMap<int, TabsProperty *> _map_settings, IDBView *db_view, IRouter *router);


public:
//    TCPController               *get_controller();
    MainTab                     *get_tab(QString name);
    QMap<QString, MainTab *>    &get_map_main_tabs();
    void                        setMap(QMap<int, TabsProperty *> map_settings);

//    virtual IController         *get_models_controller();

private:
    IDBView     *_db_view;
    QVBoxLayout *layout;
    QPushButton *_p_button;
    QLabel *_lbl0;

    QPixmap *_pm_round_red;
    QPixmap *_pm_round_green;
    bool _state;

    QMap<QString, MainTab *>    _map;
    QMap<int, TabsProperty *>   _map_spectra_keys;
    QMap<int, QString>          _map_tabs_names;

    QMap<QString, MainTab *>    _map_tabs;
    QMap<int, TabsProperty *>   _map_settings;

    IRouter                     *_router;
    ITCPController              *_tcp_controller;

//    IController                 *_models_controller;



public slots:
//    void slotChangeColor();
    void slotDMSGSetText(QString text);
    void slotCoord(int id, QPointF point);

signals:
    void write(QByteArray, int);
    void signalDMSGSetText(QString text);
    void signalCoord(int id, QPointF point);


};

#endif // MAINTABWIDGET_H
