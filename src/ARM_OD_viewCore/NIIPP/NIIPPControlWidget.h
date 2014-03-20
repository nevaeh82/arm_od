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
	void set_point(QPointF coord);

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
	void _slot_enable_complex(bool state);
	void _slot_change_value_power(int value);
	void _slot_start_stop_clicked(bool state);
	void _slot_clear();
	void _set_antenna_type(int value);
};

#endif // NIIPPCONTROLWIDGET_H
