#ifndef NIIPPCONTROLWIDGET_H
#define NIIPPCONTROLWIDGET_H

#include <QWidget>
#include "NiippController.h"

class NiippController;

namespace Ui {
class NiippControlWidget;
}

class NiippControlWidget : public QWidget
{
	Q_OBJECT

public:
	explicit NiippControlWidget(NiippController *control, QWidget *parent = 0);
	~NiippControlWidget();

	bool getState();
	void setPoint(QPointF coord);

	bool getEnableComplexState();
	int getModeIndex();
	void setStatusText(int mode);
	void setLonText(QString text);
	void setLatText(QString text);
	bool getStartState();
	int getAntenaIndex();
	int getSbPowerValue();

private:
	Ui::NiippControlWidget *ui;
	NiippController *_controller;

private slots:
	void enableComplex(bool state);
	void changeValuePower(int value);
	void startStopClicked(bool state);
	void clear();
	void setAntennaType(int value);

signals:
	void signalEnableComplex(bool);
	void signalChangeValuePower(int);
	void signalStartStopClicked(bool);
	void signalSetAntennaType(int);
	void signalChangeMode(int);
	void signalClear();
};

#endif // NIIPPCONTROLWIDGET_H
