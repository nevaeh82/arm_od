#include "MainTabWidget.h"

MainTabWidget::MainTabWidget(QWidget *parent, QMap<int, TabsProperty *> map_settings, IDBView *db_view, IRouter *router)
    : QTabWidget(parent),
      _map_settings(map_settings)
{
    this->setGeometry(QRect(10, 10, 300, 250));

    _router = router;
    _tcp_controller = _router->get_tcp_controller();
    _db_view = db_view;
    layout = new QVBoxLayout();
    layout->alignment();
    this->setLayout(layout);


    TCPParser *parser1 = new TCPParser();

    QMap<int, TabsProperty *>::iterator iter;
    for (iter = _map_settings.begin(); iter != _map_settings.end(); ++iter)
    {
        GraphicWidget *gr = new GraphicWidget(0,0,(iter.value())->get_name(), (iter.value())->get_id());

        QThread *thread = new QThread;

        QObject::connect(thread, SIGNAL(started()), gr, SLOT(proccess()));
        QObject::connect(gr, SIGNAL(finished()), thread, SLOT(quit()));
        QObject::connect(gr, SIGNAL(finished()), gr, SLOT(deleteLater()));

        QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        gr->moveToThread(thread);
        thread->start();

        (iter.value())->set_graphic_widget(gr);
        _map_spectra_keys.insert((iter.value())->get_id(), iter.value());          
    }

    _tcp_controller->add_connection(map_settings.value(1)->get_ip_Flakon(), map_settings.value(1)->get_port_Flakon(), parser1, 0);

    for (iter = _map_settings.begin(); iter != _map_settings.end(); ++iter)
    {
         _map_tabs.insert((iter.value())->get_name(), new MainTab(this, iter.value(), &_map_spectra_keys, _router, _db_view));
    }

    int i = 0;
    QMap<int, TabsProperty *>::iterator it;
    QMap<int, TabsProperty *>::iterator itt;
    for (it = _map_settings.begin(); it != _map_settings.end(); ++it)
    {
        qDebug() << it.key();        
        this->addTab(_map_tabs.value((it.value())->get_name()), (it.value())->get_name());

        qDebug() <<  "it.value()[2]" << (it.value())->get_name();
        this->tabBar()->setTabButton(i++, QTabBar::LeftSide, _map_tabs.value((it.value())->get_name())->get_indicator());


//        TCPParserReciever *parser2 = new TCPParserReciever();
//        tcp_controller.add_connection((it.value())->get_ip(), ((it.value())->get_port()), parser2);

//        _map_tabs.value((it.value())->get_name())->set_tcpclient_for_spectrum(tcp_controller.get_client((it.value())->get_ip(), 1024/*((it.value())->get_port())*/));


        _map.insert((it.value())->get_name(), _map_tabs.value((it.value())->get_name()));

        if(((it.value())->get_id()) == 1)
        {
//              connect((_map_spectra_keys.value(it.key()))->get_graphic_widget(), SIGNAL(signalRequestData(uint,uint,int*,uint)), tcp_controller.get_client((it.value())->get_ip(), ((it.value())->get_port())), SLOT(slotRequestData(uint,uint,int*,uint)));
            QObject::connect(parser1/*tcp_controller.get_client((it.value())->get_ip_Flakon(), ((it.value())->get_port_Flakon()))*//*->getParser()*/, SIGNAL(signalFFT(const QVector<QPointF>, const bool)), (_map_spectra_keys.value(it.key()))->get_graphic_widget(), SLOT(slotSetFFT(const QVector<QPointF>, const bool))/*, Qt::QueuedConnection*/);
        }
        if(((it.value())->get_id()) == 2)
        {
            QObject::connect(parser1/*tcp_controller.get_client((it.value())->get_ip_Flakon(), (it.value()->get_port_Flakon()))*/, SIGNAL(signalFFT2(const QVector<QPointF>, const bool)), (_map_spectra_keys.value(it.key()))->get_graphic_widget(), SLOT(slotSetFFT(const QVector<QPointF>, const bool))/*, Qt::QueuedConnection*/);
        }
        if(((it.value())->get_id()) == 3)
        {
            QObject::connect(parser1/*tcp_controller.get_client((it.value())->get_ip_Flakon(), ((it.value())->get_port_Flakon()))*//*->getParser()*/, SIGNAL(signalFFT3(const QVector<QPointF>, const bool)), (_map_spectra_keys.value(it.key()))->get_graphic_widget(), SLOT(slotSetFFT(const QVector<QPointF>, const bool))/*, Qt::QueuedConnection*/);
        }

        for (itt = _map_settings.begin(); itt != _map_settings.end(); ++itt)
        {            
            qDebug() << (itt.value())->get_name() << ";";

            if(it.key() != itt.key())
            {
                /// add correlations graphics
                QString name = tr("Корреляция ") + it.value()->get_name() + " - " + itt.value()->get_name();
                int id = it.key()*10 + itt.key();
                GraphicWidget *gr = new GraphicWidget(0,0, name, id);
                it.value()->set_graphic_widget_correlation(itt.key(), gr);

                QThread *thread = new QThread;

                QObject::connect(thread, SIGNAL(started()), gr, SLOT(proccess()));
                QObject::connect(gr, SIGNAL(finished()), thread, SLOT(quit()));
                QObject::connect(gr, SIGNAL(finished()), gr, SLOT(deleteLater()));

                QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

                gr->moveToThread(thread);
                thread->start();

                //TCPParser *par = tcp_controller.get_client((it.value())->get_ip_Flakon(), (it.value())->get_port_Flakon())->getParser();
                QObject::connect(parser1, SIGNAL(signalCorrelation(int,QVector<QPointF>,bool)), gr, SLOT(slotSetCorrelations(int,QVector<QPointF>,bool)));
            }
            QObject::connect(_map_tabs.value((it.value())->get_name()), SIGNAL(signalSomeAreaSelected(QString,double,double)), _map_tabs.value((itt.value())->get_name()), SLOT(slotSomeSelectedArea(QString,double,double)));

        }
        _map_tabs.value((it.value())->get_name())->set_widgets();
    }


    QObject::connect(parser1, SIGNAL(signalText(QString)), this, SLOT(slotDMSGSetText(QString)));
    QObject::connect(parser1, SIGNAL(signalCoord(int,QPointF)), this, SLOT(slotCoord(int,QPointF)));

    /// set connected signals (now only 2)
  //  connect(tcp_controller.get_client(map_settings.find(0).value()->get_ip(), (map_settings.find(0).value()->get_port())), SIGNAL(signalCorrelation1(QVector<QPointF>,bool)), )

}

//IController *MainTabWidget::get_models_controller()
//{
//    return _models_controller;
//}

//TCPController *MainTabWidget::get_controller()
//{
//    return _tcp_controller;
//}

MainTab *MainTabWidget::get_tab(QString name)
{
    return _map.value(name);
}

QMap<QString, MainTab *> &MainTabWidget::get_map_main_tabs()
{
    return _map_tabs;
}

void MainTabWidget::setMap(QMap<int, TabsProperty *> map_settings)
{
    _map_settings = map_settings;
}

/// transcend method for translate message from polymorth ITCPParser to diagnosticMSG form
void MainTabWidget::slotDMSGSetText(QString text)
{
    emit signalDMSGSetText(text);
}

/// transcend method for translate message from polymorth ITCPParser to diagnosticMSG
void MainTabWidget::slotCoord(int id, QPointF point)
{
    emit signalCoord(id, point);
}
