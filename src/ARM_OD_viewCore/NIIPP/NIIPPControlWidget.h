#ifndef NIIPPCONTROLWIDGET_H
#define NIIPPCONTROLWIDGET_H

#include <QWidget>
#include "NIIPPControl.h"

namespace Ui {
class NIIPPControlWidget;
}

class NIIPPControl;

class NIIPPControlWidget : public QWidget
{
	Q_OBJECT

public:
	explicit NIIPPControlWidget(NIIPPControl *control, QWidget *parent = 0);
	~NIIPPControlWidget();

	bool getState();
	void setPoint(QPointF coord);

	bool getEnableComplexState();
	int getModeIndex();
	void setLeStatusText(int mode);
	void setLonText(QString text);
	void setLatText(QString text);
	bool getStartState();
	int getAntenaIndex();
	int getSbPowerValue();

private:
	Ui::NIIPPControlWidget *ui;
	NIIPPControl *_controller;

private slots:
	void slotEnableComplex(bool state);
	void slotChangeValuePower(int value);
	void slotStartStopClicked(bool state);
	void slotClear();
	void setAntennaType(int value);
};

#endif // NIIPPCONTROLWIDGET_H
