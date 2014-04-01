#include "TabsController.h"
#include <QDebug>

TabsController::TabsController()
{
}

TabsController::~TabsController()
{
}

/// create common components from ini faile [CommonComponents]
int TabsController::createCommonComponents()
{
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QString app_dir = QCoreApplication::applicationDirPath();
    app_dir.append("/CommonComponents.ini");
    QSettings m_settings(app_dir, QSettings::IniFormat);

    m_settings.setIniCodec(codec);

    QStringList childKeys = m_settings.childGroups();
    foreach (const QString &childKey, childKeys)
    {
        qDebug() << "m_settings = " << childKey.toLatin1();
//        m_settings.beginGroup(childKey);
//        if(childKey.toLatin1() == "ServerM")
//        {
//            ip = m_settings.value("IP", "127.0.0.1").toString();
//            port = m_settings.value("Port", 1024).toInt();
//            m_settings.endGroup();
//            continue;
//        }

//        TabsProperty *prop = new TabsProperty();

//        prop->set_ip_Flakon(ip);
//        prop->set_port_Flakon(port);

//        prop->set_id(m_settings.value("Id", 0).toInt());
//        prop->set_name(m_settings.value("Name", 0).toString());
//        prop->set_ip(m_settings.value("IP", "127.0.0.1").toString());
//        prop->set_port(m_settings.value("Port", 2323).toInt());
//        prop->set_latitude(m_settings.value("Latitude", "0").toDouble());
//        prop->set_longitude(m_settings.value("Longitude", "0").toDouble());

//        _map_settings.insert(m_settings.value("Id", 0).toInt(), prop/*values*/);
//        m_settings.endGroup();
    }
}
