#ifndef TABSCONTROLLER_H
#define TABSCONTROLLER_H

#include <QCoreApplication>

#include <QTextCodec>
#include <QSettings>

class TabsController
{
public:
    TabsController();
    ~TabsController();

public:
    int createCommonComponents();
};

#endif // TABSCONTROLLER_H
