#ifndef SOLVERCONTROLLER_H
#define SOLVERCONTROLLER_H

#include <QObject>

#include "SolverWidget.h"
#include "Solver.h"
#include "../ITabManager.h"

class SolverController : public QObject
{
	Q_OBJECT

public:
	SolverController(int id, ITabManager* tab_manager);
	~SolverController();

	QWidget* getWidget();
	Solver* getModel();

private:
	SolverWidget *m_widget;
	Solver       *m_model;

public slots:
	void show();

private slots:
	void cancel();
	void accept();
	void autoState(bool);
	void clear();
};

#endif // SOLVERCONTROLLER_H
