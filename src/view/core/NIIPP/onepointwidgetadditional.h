#ifndef ONEPOINTWIDGETADDITIONAL_H
#define ONEPOINTWIDGETADDITIONAL_H

#include <QWidget>
#include "nstation.h"

namespace Ui {
class onepointwidgetadditional;
}

class onepointwidgetadditional : public QWidget
{
    Q_OBJECT

public:
    explicit onepointwidgetadditional(NStation cfg, QWidget *parent = 0);
    ~onepointwidgetadditional();

    NStation getParams();
    void setParams(NStation cfg);

private:
    Ui::onepointwidgetadditional *ui;

signals:
    void paramChanged();
};

#endif // ONEPOINTWIDGETADDITIONAL_H
