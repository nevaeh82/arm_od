#include "MainTab.h"
#include <QDebug>

MainTab::MainTab(QWidget *parent, TabsProperty *property, QMap<int, TabsProperty *> *map, IRouter *router, IDBView *db_view) :
    QWidget(parent),
    _name_view_reciever(tr("Настройки"))
{

    _router = router;
    _models_controller = _router->get_models_controller();
    _db_view = db_view;
    qDebug() << "NAME = " << property->get_id();
    _map_spectra = map;
    _property = property;
    _id_name = property->get_name();

    _id_spectra = _property->get_id();

    _tcp_client = _router->get_tcp_controller()->get_client(property->get_ip(), property->get_port(), 0);

    _pm_round_red = new QPixmap(":/images/signals/images/bullet_red.png");
    _pm_round_green = new QPixmap(":/images/signals/images/bullet_green.png");
    _indicator = new QLabel();
    _indicator->setFixedSize(16, 16);
    _indicator->setPixmap(_pm_round_red->scaled(16,16,Qt::KeepAspectRatio));
    _main_view_stacked_widget = new QStackedWidget();
    _main_view_stacked_widget->setContentsMargins(0, 0, 0, 0);

    hbox2 = new QHBoxLayout();

    hbox2->insertWidget(0, _main_view_stacked_widget, Qt::AlignJustify);

    _main_map_widget = new QWidget();
    _main_view_stacked_widget->addWidget(_main_map_widget);

    _view = new MainTabView();
    _view->setMinimumWidth(200);
    _view->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

    _view->get_tree()->setMinimumWidth(200);

    _control_widget = new ControlPanelWidgets(_view->get_tree());
    connect(_control_widget->tb_map, SIGNAL(clicked()), this, SLOT(slotShowMap()));
    connect(_control_widget->tb_spectrum, SIGNAL(clicked()), this, SLOT(slotShowSpetrum()));
    connect(_control_widget->tb_spectra, SIGNAL(clicked()), this, SLOT(slotShowSpetra()));
    connect(_control_widget->tb_correlations, SIGNAL(clicked()), this, SLOT(slotShowCorrelations()));

    ButtonShowPanel *pb = new ButtonShowPanel(_view->get_tree());
    pb->setMaximumHeight(30);
    pb->setMaximumWidth(30);
    pb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QGridLayout *gbox2 = new QGridLayout(_view->get_tree());
    gbox2->setContentsMargins(0, 0, 0, 0);
    gbox2->setSpacing(0);
    gbox2->addWidget(pb, 0, 0, Qt::AlignRight | Qt::AlignTop);

    pb->setMouseTracking(true);

    _view->get_tree()->setLayout(gbox2);

    connect(pb, SIGNAL(signalPanelShow()), _control_widget, SLOT(slotShow()));

    hbox2->insertWidget(1, _view, Qt::AlignRight);

    connect(_view, SIGNAL(signalResize(int,int)), _control_widget, SLOT(slotSetSize(int,int)));

    this->setLayout(hbox2);

//    _exchanger_spectrum = new ExchangeMainTreeItem(_tcp_client);
//    _exchanger_spectrum->set_id(_id_spectra);

//    connect(_exchanger_spectrum, SIGNAL(signalWriteData(QByteArray)), client, SLOT(writing(QByteArray)));

    connect(_property->get_graphic_widget(), SIGNAL(signalEnableKM(bool)), this, SLOT(slotEnableKM(bool)));

//    _set_punkt_settings();

    connect(this, SIGNAL(signalSetAreaSelected(double,double, double, double)), this, SLOT(slotSelectedArea(double,double, double, double)));
    connect(this, SIGNAL(signalSetThreshold(double)), this, SLOT(slotSetThreshold(double)));

    _view->get_tree()->header()->setResizeMode(QHeaderView::ResizeToContents);

//    this->set_widgets();
//    this->slotShowMap();

    ITCPController *contr = _router->get_tcp_controller();
    ITCPClient *cl = contr->get_client(_property->get_ip_Flakon(), _property->get_port_Flakon(), 0);
    QObject::connect(cl, SIGNAL(signalPing(bool)), this, SLOT(slotChangeColor(bool)));
    slotChangeColor(cl->get_state());

}

///// set initial settings for tree view for punkts
//void MainTab::_set_punkt_settings()
//{
//    _view->insertRowSettingProperties(_name_view_reciever, tr("Центр f (МГц)"), "0");
//    _view->insertRowSettingProperties(_name_view_reciever, tr("Фильтр (кГц)"), "0");
//    _view->insertRowSettingProperties(_name_view_reciever, tr("Сдвиг (кГц)"), "0");
//}

MainTab::~MainTab()
{
    delete _pm_round_red;
    delete _pm_round_green;
    delete _indicator;
    delete _main_view_stacked_widget;
}

/// set map widget (it have to be only one copy)!
void MainTab::set_map_widget()
{
   _set_map_widget();
}

/// set widgets to _main_view_stacked_widget
void MainTab::set_widgets()
{
    _set_spectrum_widget();
    _set_spectra_widget();
    _set_correlations_widget();
}

void MainTab::_set_map_widget()
{
    gl_map = new QGridLayout();

    _map_contr->get_widget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    gl_map->addWidget(_map_contr->get_widget(), 0, 0, 0, 0, 0);
    _main_map_widget->setLayout(gl_map);

    _state = false;

	connect(_map_contr, SIGNAL(mapOpened()), this, SLOT(slotShowMap()));
//    connect(_tcp_client, SIGNAL(signalCoord(int, QPointF)), _map_contr->get_map_client(_id_spectra), SLOT(slotAddBLA_evil(int, QPointF)));
    connect(_map_contr->get_map_client(_id_spectra), SIGNAL(signalNewBPLA(QString)), this, SLOT(slotSetBPLAItem(QString)));
}

void MainTab::_add_map_to_stackwidget()
{
//    _main_view_stacked_widget->addWidget(_main_map_widget);
}

void MainTab::_set_spectrum_widget()
{
    _main_spectrum_widget = new QWidget();
    //_chb_setThreshold = new QCheckBox(_main_spectrum_widget);
    //_chb_setThreshold->setText(tr("Установить порог"));
    _gl_spectrum = new QGridLayout();

    _grafic_widget_one = _property->get_graphic_widget();//_map_spectra->value(_id_spectra);//new GraphicWidget(this);


    connect(_grafic_widget_one, SIGNAL(selected(double, double, double, double)), this, SLOT(slotSelectedArea(double, double, double,double)));
    connect(_grafic_widget_one, SIGNAL(signalChoosedThreshold(double)), this, SLOT(slotSetThreshold(double)));
    connect(_grafic_widget_one, SIGNAL(signalAddSelToLists(int)), this, SLOT(slotAddToDBList(int)));
    _gl_spectrum->addWidget(_grafic_widget_one, 0, 0/*, Qt::AlignVCenter*/);
    //_gl_spectrum->addWidget(_chb_setThreshold, 1, 0, Qt::AlignLeft | Qt::AlignTop);

    _main_spectrum_widget->setLayout(_gl_spectrum);
    _main_view_stacked_widget->addWidget(_main_spectrum_widget);
    _main_view_stacked_widget->setCurrentIndex(0);

//    _view->setModel(_model_spectrum);
}

void MainTab::_set_spectra_widget()
{
    _main_spectra_widget = new QWidget();
    _gl_spectra = new QGridLayout();

    int i = 0;
    int j = 0;
    bool state = false;
    //QMap<int, GraphicWidget *>::iterator it;
    QMap<int, TabsProperty *>::iterator it;
    for (it = (*_map_spectra).begin(); it != (*_map_spectra).end(); ++it)
    {
//         connect(it.value(), SIGNAL(selected(double, double)), this, SLOT(slotSelectedArea(double, double))); 
        _gl_spectra->addWidget((it.value())->get_graphic_widget(), i, j, Qt::AlignLeft | Qt::AlignTop);

         if(state == false)
         {
            j++;
            state = true;
         }
         else
         {
             j = 0;
             i++;
             state = false;
         }
    }

    _main_spectra_widget->setLayout(_gl_spectra);
    _main_view_stacked_widget->addWidget(_main_spectra_widget);
    _main_view_stacked_widget->setCurrentIndex(0);
}

void MainTab::_set_correlations_widget()
{
    _main_correlations_widget = new QWidget();
    _gl_correlations = new QGridLayout();

    int i = 0;
    int j = 0;
    bool state = false;

    QMap<int, GraphicWidget *>::iterator it;
    for(it = _property->get_correlations().begin(); it != _property->get_correlations().end(); ++it)
    {
        _gl_correlations->addWidget(it.value(), i, j);

        if(state == false)
        {
           j++;
           state = true;
        }
        else
        {
            j = 0;
            i++;
            state = false;
        }
    }
    _main_correlations_widget->setLayout(_gl_correlations);
    _main_view_stacked_widget->addWidget(_main_correlations_widget);
    _main_view_stacked_widget->setCurrentIndex(0);
}

QLabel *MainTab::get_indicator()
{
    qDebug() << "INdicator" << this->_id_name;
    return _indicator;
}

QPushButton *MainTab::get_push_button()
{
    return _p_button;
}

void MainTab::slotChangeColor(bool state)
{
    _state = state;
    if(_state == true)
    {
      _indicator->setPixmap(_pm_round_green->scaled(16,16,Qt::KeepAspectRatio));
    }
    else
    {
      _indicator->setPixmap(_pm_round_red->scaled(16,16,Qt::KeepAspectRatio));
    }

//    _view->updateState(_id_name, state);
}

void MainTab::set_map_controller(MapController *map_contr)
{
    _map_contr = map_contr;
}

QLayout *MainTab::get_map_layout()
{
    return _main_map_widget->layout();
}

void MainTab::set_map_layout(QLayout *map_layout)
{
    _main_map_widget->setLayout(map_layout);
}

QWidget *MainTab::get_map_widget()
{
    return _main_map_widget;
}

void MainTab::set_map_widget(QWidget *map_widget)
{
    _main_map_widget = map_widget;
    _main_map_widget->setLayout(map_widget->layout());
}

void MainTab::enterEvent(QEvent *event)
{
   //_control_widget->show();
}

void MainTab::leaveEvent(QEvent *event)
{
    //_control_widget->hide();
}

/// slot for showing map widget
void MainTab::slotShowMap()
{
//    _view_settings->hide();
//    _view->setModel(_model_map);
    int id_view = 1;
    _models_controller->reset_model(0, id_view);
    _models_controller->setupData();
    _view->setModel(_models_controller->get_model());
    _main_view_stacked_widget->setCurrentIndex(0);
}

/// slot for showing spectrum widget
void MainTab::slotShowSpetrum()
{
    /// set for model type of view
    int id_view = 1;
    TabsProperty *tab = _map_spectra->value(_id_spectra);
    GraphicWidget *gr = tab->get_graphic_widget();
    _gl_spectrum->removeWidget(gr);
    _gl_spectrum->addWidget(gr, 0, 0/*, Qt::AlignVCenter*/);
//    _view->setModel(_model_spectrum);

    _models_controller->reset_model(_id_spectra, id_view);
    _models_controller->setupData();
    _view->setModel(_models_controller->get_model());
    _main_view_stacked_widget->setCurrentIndex(1);

//    _view_settings->show();

}

/// slot for showing spectra widget
void MainTab::slotShowSpetra()
{
//    _view_settings->hide();

    TabsProperty *tab = _map_spectra->value(_id_spectra);
    GraphicWidget *gr = tab->get_graphic_widget();

    _gl_spectra->removeWidget(gr);

    int i = 0;
    int j = 0;
    bool state = false;
//    QMap<int, GraphicWidget *>::iterator it;
    QMap<int, TabsProperty *>::iterator it;
    for (it = (*_map_spectra).begin(); it != (*_map_spectra).end(); ++it)
    {
//         connect(it.value(), SIGNAL(selected(double, double)), this, SLOT(slotSelectedArea(double, double)));
        tab = _map_spectra->value(it.key());
        _gl_spectra->addWidget(tab->get_graphic_widget(), i, j);

//    for (it = (*_map_spectra).begin(); it != (*_map_spectra).end(); ++it)
//    {
//         qDebug() << it.key();
//         //connect(it.value(), SIGNAL(selected(double, double)), this, SLOT(slotSelectedArea(double, double)));
//         _gl_spectra->addWidget(it.value(), i, j/*, Qt::AlignLeft | Qt::AlignTop*/);

         if(state == false)
         {
            j++;
            state = true;
         }
         else
         {
             j = 0;
             i++;
             state = false;
         }
    }
//    _view->setModel(_model_spectra);
    /// set for model type of view
    int id_view = 2;

    _models_controller->reset_model(0, id_view);
    _models_controller->setupData();
    _view->setModel(_models_controller->get_model());
    _main_view_stacked_widget->setCurrentIndex(2);
}

void MainTab::slotShowCorrelations()
{
    /// set for model type of view
    int id_view = 3;
    _models_controller->reset_model(0, id_view);
    _models_controller->setupData();
    _view->setModel(_models_controller->get_model());
    _main_view_stacked_widget->setCurrentIndex(3);
}

void MainTab::slotSelectedArea(double x1, double x2, double y1, double y2)
{
    double dx = x2 - x1;
    //double dy = y2 - y1;
    //slotSetThreshold(0);
    qDebug() << "SELECTED: " << _id_name;

    double center_freq = dx/2 + x1;
    _models_controller->update_property(_id_spectra, 1/*tr("Выделено")*/, dx);
    _models_controller->update_property(_id_spectra, 2/*tr("Центр")*/, center_freq);
    _models_controller->update_property(_id_spectra, 3/*tr("Начало")*/, x1);
    _models_controller->update_property(_id_spectra, 4/*tr("Конец")*/, x2);
    qDebug() << "UPDATES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    _view->get_tree()->expandAll();
//    _view->update();
//    bool error = _view->itemPresent(_id_name, tr("Центр"), index);
//    if(error)
//    {
//        _view->updateItem(index, QString::number(center_freq, 'f', 6), tr("Центр"));
//    }
//    else
//    {
//        _view->insertRowProperties(_id_name, tr("Центр"), QString::number(center_freq, 'f', 6));
//    }

//    error = _view->itemPresent(_id_name, tr("Выделено"), index);
//    if(error)
//    {
//        _view->updateItem(index, QString::number(dx, 'f', 6), tr("Выделено"));
//    }
//    else
//    {
//        _view->insertRowProperties(_id_name, tr("Выделено"), QString::number(dx, 'f', 6));
//    }

//    error = _view->itemPresent(_id_name, tr("Начало"), index);
//    if(error)
//    {
//        _view->updateItem(index, QString::number(x1, 'f', 6), tr("Начало"));
//    }
//    else
//    {
//        _view->insertRowProperties(_id_name, tr("Начало"), QString::number(x1, 'f', 6));
//    }

//    error = _view->itemPresent(_id_name, tr("Конец"), index);
//    if(error)
//    {
//        _view->updateItem(index, QString::number(x2, 'f', 6), tr("Конец"));
//    }
//    else
//    {
//        _view->insertRowProperties(_id_name, tr("Конец"), QString::number(x2, 'f', 6));
//    }

//    _view->setModel(old_model);

//    emit signalSomeAreaSelected(_id_name, dx, dy);
    //emit signalSetThreshold(0);
}


/// view selected area in model spectra
void MainTab::slotSomeSelectedArea(QString id, double dx, double dy)
{
//    qDebug() << "SELECTED3 = " << id;
//    MainTreeModelManager *old_model = dynamic_cast<MainTreeModelManager *>(_view->get_tree()->model());
////    _view->setModel(_model_spectra);
//    QModelIndex index;
//    bool error = _view->itemPresent(id, tr("Выделено"), index);
//    if(error)
//    {
//        qDebug() << "ERROR" << _id_name;
//        _view->updateItem(index, QString::number(dx, 'f', 6), tr("Выделено"));
//    }
//    else
//    {
//        qDebug() << "INSERT " << id;
//        _view->insertRowProperties(id, tr("Выделено"), QString::number(dx, 'f', 6));
//    }
//    _view->get_tree()->expandAll();
//    _view->setModel(old_model);

//    if(current_index_stack_widget() == 1)
//    {
//        _view->get_tree()->expandAll();
//    }
}

/// get current stacked widget index
int MainTab::current_index_stack_widget()
{
    return _main_view_stacked_widget->currentIndex();
}

/// show threshold
void MainTab::slotSetThreshold(double y)
{    
//    MainTreeModelManager *old_model = dynamic_cast<MainTreeModelManager *>(_view->get_tree()->model());
////    _view->setModel(_model_spectrum);
//    QModelIndex index;
//    bool error = _view->itemPresent(_id_name, tr("Порог"), index);
//    if(error)
//    {
//        _view->updateItem(index, QString::number(y, 'f', 6), tr("Порог"));
//    }
//    else
//    {
//        _view->insertRowProperties(_id_name, tr("Порог"), QString::number(y, 'f', 6));
//    }
//    _view->get_tree()->expandAll();
//    _view->setModel(old_model);
//    emit signalSomeSetThreshold(_id_name, y);
//    //emit signalSetAreaSelected(0, 0);
}

/// set threshold for all
void MainTab::slotSomeSetThreshold(QString id, double y)
{
//    MainTreeModelManager *old_model = dynamic_cast<MainTreeModelManager *>(_view->get_tree()->model());
////    _view->setModel(_model_spectra);
//    QModelIndex index;
//    bool error = _view->itemPresent(id, tr("Порог"), index);
//    if(error)
//    {
//        _view->updateItem(index, QString::number(y, 'f', 6), tr("Порог"));
//    }
//    else
//    {
//        _view->insertRowProperties(id, tr("Порог"), QString::number(y, 'f', 6));
//    }
//    _view->get_tree()->expandAll();
//    _view->setModel(old_model);

}

/// get tab name
QString MainTab::get_name()
{
    return _id_name;
}

/// set BPLA item in model
void MainTab::slotSetBPLAItem(QString bpla)
{
//    MainTreeModelManager *old_model = dynamic_cast<MainTreeModelManager *>(_view->get_tree()->model());
////    _view->setModel(_model_map);
////    QModelIndex index;
////    bool error = _view->itemPresent(_id_spectra, bpla, index);
////    if(error)
////    {
////        //_view->updateItem(index, QString::number(y, 'f', 6), tr("Порог"));
////    }
////    else
////    {
//        _view->insertRootChild(bpla, Qt::CheckStateRole);
////    }
//    _view->get_tree()->expandAll();
//    _view->setModel(old_model);
}

/// add selection to db of list
void MainTab::slotAddToDBList(int id_list)
{
//    MainTreeModelManager *old_model = dynamic_cast<MainTreeModelManager *>(_view->get_tree()->model());
////    _view->setModel(_model_spectrum);

//    double freq = 0;
//    double brandwidth = 0;

//    QModelIndex index;
//    bool error = _view->itemPresent(_id_name, tr("Центр"), index);
//    if(error)
//    {
//        QModelIndex index1 = index.sibling(index.row(), 1);
//        if(!index1.isValid())
//        {
//            qDebug() << "invalid";
//        }
//        else
//        {
//            QVariant var = _view->get_tree()->model()->data(index1, Qt::DisplayRole);
//            freq = var.toDouble();
//            qDebug() << "FREQUENCY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! = " << freq;
//        }
//    }

//    error = _view->itemPresent(_id_name, tr("Выделено"), index);
//    if(error)
//    {
//        QModelIndex index2 = index.sibling(index.row(), 1);
//        if(!index2.isValid())
//        {
//            qDebug() << "invalid";
//        }
//        else
//        {
//            QVariant var2 = _view->get_tree()->model()->data(index2, Qt::DisplayRole);
//            brandwidth = var2.toDouble();
//            qDebug() << "brandwidth!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! = " << brandwidth;
//        }
//    }

//    SignalBDDescription sig;
//    sig.host = _property->get_ip();
//    sig.port = _property->get_port();
//    sig.id_device = _property->get_id();
//    sig.freq = freq;
//    sig.brandwidth = brandwidth;
//    sig.type_transmition = 0;
//    sig.time = QDateTime::currentDateTime();

//    QString table_view;
//    switch(id_list)
//    {
//    case 1:
//        table_view = TABLE_WHITELIST;
//        break;
//    case 2:
//        table_view = TABLE_BLACKLIST;
//        break;
//    default:
//        break;
//    }

//    _db_view->appendSingleData(table_view, sig);
}

/// set KM (coordinate meter) state
void MainTab::slotEnableKM(bool state)
{
    QString str_name = "KM";
    QVariant name(str_name);
    QVariant value(state);
//    _exchanger_spectrum->push_data(name, value);
}
