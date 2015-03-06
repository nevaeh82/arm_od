#include "IconsGenerator.h"

IconsGenerator::IconsGenerator()
{
}

IconsGenerator::~IconsGenerator()
{
}

/// set centred widget with icon
QPushButton *IconsGenerator::set_centred()
{
    QPixmap *_pm_icon = new QPixmap(":/images/map/images/Tabs/map.png");
    QPushButton *indicator = new QPushButton();
    indicator->setFixedSize(30, 30);
    indicator->setIcon(QIcon(*_pm_icon));
    //indicator->setPixmap(_pm_round_red->scaled(50,50,Qt::KeepAspectRatio));
    return indicator;
}

QPushButton *IconsGenerator::set_home()
{
    QPixmap *_pm_icon = new QPixmap(":/images/map/images/Tabs/bank_128.png");
    QPushButton *bank = new QPushButton();
    bank->setFixedSize(30, 30);
    bank->setIcon(QIcon(*_pm_icon));
    //indicator->setPixmap(_pm_round_red->scaled(50,50,Qt::KeepAspectRatio));
    return bank;
}

QPushButton *IconsGenerator::set_fix()
{
    QPixmap *_pm_icon = new QPixmap(":/images/map/images/Tabs/fix_128.png");
    QPushButton *fix = new QPushButton();
    fix->setFixedSize(30, 30);
    fix->setIcon(QIcon(*_pm_icon));
    return fix;
}

QPushButton *IconsGenerator::set_spiral()
{
    QPixmap *_pm_icon = new QPixmap(":/images/map/images/Tabs/spiral_zoom_128.png");
    QPushButton *fix = new QPushButton();
    fix->setFixedSize(30, 30);
    fix->setIcon(QIcon(*_pm_icon));
    return fix;
}

QPushButton *IconsGenerator::set_tablet()
{
    QPixmap *_pm_icon = new QPixmap(":/images/map/images/Tabs/tablet_128.png");
    QPushButton *fix = new QPushButton();
    fix->setFixedSize(30, 30);
    fix->setIcon(QIcon(*_pm_icon));
    return fix;
}

QPushButton *IconsGenerator::set_web_mail()
{
    QPixmap *_pm_icon = new QPixmap(":/images/map/images/Tabs/web_mail_128.png");
    QPushButton *fix = new QPushButton();
    fix->setFixedSize(30, 30);
    fix->setIcon(QIcon(*_pm_icon));
    return fix;
}
