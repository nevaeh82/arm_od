#ifndef SOLVERCONTROLLER_H
#define SOLVERCONTROLLER_H

#include <QObject>

#include "SolverSettingsDialog.h"
#include "../ITabManager.h"

#include <Interfaces/IController.h>

#include "Common/CommandMessage.h"

#define SOLVERSETTINGDIALOGCANCEL 0
#define SOLVERSETTINGDIALOGACCEPT 1
#define SOLVERSETTINGDIALOGCLEAR 2

class SolverSettingsDialogController : public QObject, public IController<SolverSettingsDialog>
{
	Q_OBJECT
private:
	SolverSettingsDialog* m_view;
	ITabManager* m_tabManager;
	int m_id;

	bool m_autoStateValue;
	double m_height;
	int m_trackLength;
	int m_countNum;

public:
	SolverSettingsDialogController(int id, ITabManager* tab_manager, QObject* parent = NULL);
	virtual ~SolverSettingsDialogController();

	void appendView(SolverSettingsDialog* view);

private:
	void setAutoState(bool val);
	void setHeight(double val);
	void setTrackLength(int val);
	void setCountNum(int val);

private slots:
	void cancel();
	void accept();
	void autoState(bool);
	void clear();
};

#endif // SOLVERCONTROLLER_H
