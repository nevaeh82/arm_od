#ifndef MAINTAB_H
#define MAINTAB_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTreeView>
#include <QStackedWidget>

#include "MainTabView.h"
#include "../TCP/TCPController.h"
#include "../MapInterface/MapController.h"
#include "../Controls/ControlPanelWidgets.h"
#include "../Controls/ButtonShowPanel.h"

#include "../Graphics/GraphicWidget.h"

#include "ExchangeMainTreeItem.h"

#include "TabsProperty.h"

//#include "../../ZaviruhaCore/BD/BDComSqlite.h"

#include "../DBForms/IDBView.h"

#include "Controller.h"

#include "../Interfaces/core/IRouter.h"
#include "../Interfaces/ITCPClient.h"
#include "../Interfaces/core/ITCPController.h"

class MainTab : public QWidget
{
    Q_OBJECT
public:
    explicit MainTab(QWidget *parent = 0, TabsProperty *t = NULL, QMap<int, TabsProperty *> *map = NULL, IRouter *router = NULL, IDBView *db_view = NULL);
    ~MainTab();

public:
    QLabel *get_indicator();
    QPushButton *get_push_button();
    void set_map_controller(MapController *map_contr);

    QLayout *get_map_layout();
    void set_map_layout(QLayout *map_layout);

    void set_widgets();

    QWidget *get_map_widget();
    void set_map_widget(QWidget *map_widget);

    void _add_map_to_stackwidget();

    void set_map_widget();

    int current_index_stack_widget();

    QString get_name();

//    void set_tcpclient_for_spectrum(ITCPClient *client);

private:
    IDBView         *_db_view;
    TabsProperty    *_property;
    QString         _id_name;
    const QString   _name_view_reciever;
    int             _id_spectra;
    QMap<int, TabsProperty *>      *_map_spectra;
//    QMap<int, QString>              *_map_spectra_names;
    QStackedWidget  *_main_view_stacked_widget;
    QWidget         *_main_map_widget;
    QWidget         *_main_spectrum_widget;
    QWidget         *_main_spectra_widget;
    QWidget         *_main_correlations_widget;


    QLabel          *_indicator;
    QPushButton     *_p_button;
    MainTabView     *_view;
//    MainTabView     *_view_settings;
    QTreeView       *_tree_view;

//    MainTabView     *_view2;

    QHBoxLayout     *hbox2;
    QGridLayout     *gl_map;


    QPixmap         *_pm_round_red;
    QPixmap         *_pm_round_green;
    bool            _state;

//    TCPController   _tcp_controller;

	MapController   *_map_contr;

    ControlPanelWidgets *_control_widget;

    GraphicWidget       *_grafic_widget_one;

    MainTreeModelManager    *_model_map;

    MainTreeModelManager    *_model_spectrum;

    MainTreeModelManager    *_model_spectra;

    MainTreeModelManager    *_model_spectrum_settings;

    QGridLayout         *_gl_spectrum;
    QGridLayout         *_gl_spectra;
    QGridLayout         *_gl_correlations;

    QCheckBox           *_chb_setThreshold;

//    ExchangeMainTreeItem    *_exchanger_spectrum;
    ITCPClient              *_tcp_client;

    IController              *_models_controller;

    IRouter                 *_router;

private:
    void _set_map_widget();
    void _set_spectra_widget();
    void _set_spectrum_widget();
    void _set_correlations_widget();

//    void _set_punkt_settings();

private:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

    
signals:
    void signalSetAreaSelected(double, double);
    void signalSetThreshold(double);
    void signalSomeAreaSelected(QString, double, double);
    void signalSomeSetThreshold(QString, double);

    
public slots:
    void slotChangeColor(bool);
    void slotShowMap();
    void slotShowSpetrum();
    void slotShowSpetra();
    void slotShowCorrelations();

    void slotSelectedArea(double x1, double x2, double y1, double y2);
    void slotSetThreshold(double y);
    void slotSomeSelectedArea(QString, double, double);
    void slotSomeSetThreshold(QString, double);

    void slotSetBPLAItem(QString bpla);

    void slotAddToDBList(int id_list);

    void slotEnableKM(bool state);
    
};

#endif // MAINTAB_H
