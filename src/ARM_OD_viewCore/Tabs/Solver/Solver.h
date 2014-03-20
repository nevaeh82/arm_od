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

namespace Ui {
class Solver;
}

class Solver : public QWidget
{
	Q_OBJECT

public:
	explicit Solver(int id, ITabManager* tab_manager);
	~Solver();

	QWidget* getWidget();

private:
	SolverWidget*       _widget;
	ITabManager*        _tab_manager;
	int                 _id;

private slots:
	void slotAccept();
	void slotCancel();
	void slotAuto(bool state);

	void slotClear();
	void slotCountTrack(int count);

public slots:
	void slotShow();
};

#endif // SOLVER_H
