#ifndef NIIPPCONTROLWIDGET_H
#define NIIPPCONTROLWIDGET_H

#include <QWidget>
#include "Niipp.h"

namespace Ui {
class NiippWidget;
}

class NiippWidget : public QWidget
{
	Q_OBJECT

public:
	explicit NiippWidget(QWidget *parent = 0);
	~NiippWidget();

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
	void changeValuePower(int value, double radiusCircle, double radiusSector, int antennaType);
	void setAntennaType(int value, Niipp::WorkMode workMode);

private:
	Ui::NiippWidget *ui;

private slots:
	void enableComplex(bool state);
	void clear();
	void start(bool state);


signals:
	void complexEnabled(bool);
	void valuePowerChanged(int);
	void startClicked();
	void stopClicked();
	void antennaTypeChanged(int);
	void modeChanged(int);
	void cleared();
};

#endif // NIIPPCONTROLWIDGET_H
