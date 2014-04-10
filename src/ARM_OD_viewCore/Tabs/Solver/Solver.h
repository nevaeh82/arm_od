#ifndef SOLVER_H
#define SOLVER_H

#include "Tabs/ITabManager.h"
#include "Common/CommandMessage.h"

class Solver
{

public:
	explicit Solver(int id, ITabManager* tabManager);
	~Solver();

	void setAutoState(bool val);
	void setHeight(double val);
	void setTrackLength(int val);

	void autoState();
	void accept();
	void clear();

private:
	ITabManager*        m_tabManager;
	int                 m_id;

	bool m_autoStateValue;
	double m_height;
	int m_trackLength;

	void countTrack(int count);
};

#endif // SOLVER_H
