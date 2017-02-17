#ifndef KTRADRESSCONTROL_H
#define KTRADRESSCONTROL_H

#include <QWidget>

#include "IpAddressControl.h"

namespace Ui {
class KtrAdressControl;
}

class KtrAdressControl : public QWidget
{
	Q_OBJECT

public:
	explicit KtrAdressControl(QWidget *parent = 0);
	~KtrAdressControl();

private:
	Ui::KtrAdressControl *ui;
	void init();

private slots:
	void slotApply();
};

#endif // KTRADRESSCONTROL_H
