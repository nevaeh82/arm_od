#ifndef MAINTABBAR_H
#define MAINTABBAR_H

#include <QTabBar>
#include <QLabel>

class MainTabBar : public QTabBar
{
public:
    MainTabBar(QWidget *parent);

private:
    void _setIndicate(bool state);

private:
    QPixmap *_pm_round_red;
    QPixmap *_pm_round_green;
};

#endif // MAINTABBAR_H
