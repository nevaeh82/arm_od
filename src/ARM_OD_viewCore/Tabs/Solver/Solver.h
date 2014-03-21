#ifndef SOLVER_H
#define SOLVER_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QByteArray>
#include <QDataStream>
#include <QCheckBox>

#include "SolverWidget.h"

#include "../ITabManager.h"
#include "../../Common/CommandMessage.h"

class Solver : public QObject
{
	Q_OBJECT

public:
	explicit Solver(int id, ITabManager* tab_manager);
	~Solver();

	void setAutoState(bool val);
	void setHeight(double val);
	void setTrackLength(int val);

	void autoState();
	void accept();

private:
	ITabManager*        _tab_manager;
	int                 _id;

	bool _autoStateValue;
	double _height;
	int _trackLength;

private slots:
	void clear();
	void countTrack(int count);

public slots:
};

#endif // SOLVER_H
