#pragma once

#include <QWidget>

#include "IpAddressControl.h"

namespace Ui {
class PvoAddressControl;
}

class PvoAddressControl : public QWidget
{
	Q_OBJECT

public:
    explicit PvoAddressControl(QWidget *parent = 0);
    ~PvoAddressControl();

private:
    Ui::PvoAddressControl *ui;
	void init();

private slots:
	void slotApply();

signals:
    void applySignal(QString, int, int);
};
