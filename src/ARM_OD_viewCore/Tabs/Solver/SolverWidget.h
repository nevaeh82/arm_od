#ifndef SOLVERWIDGET_H
#define SOLVERWIDGET_H

#include <QWidget>

namespace Ui {
	class SolverWidget;
}

class SolverWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SolverWidget(QWidget *parent = 0);

	int getTrackLength();
	double getHeight();
	int getCountNum();
	bool getAutoState();

private:
	Ui::SolverWidget *_ui;

signals:
	void signalClear();
	void signalAccept();
	void signalCancel();
	void signalAuto(bool);
};

#endif // SOLVERWIDGET_H
