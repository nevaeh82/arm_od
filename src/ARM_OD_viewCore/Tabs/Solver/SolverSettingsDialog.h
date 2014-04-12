#ifndef SOLVERSETTINGSDIALOG_H
#define SOLVERSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SolverSettingsDialog;
}

class SolverSettingsDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::SolverSettingsDialog *ui;
	
public:
	explicit SolverSettingsDialog(QWidget *parent = 0);
	~SolverSettingsDialog();

	int getTrackLength();
	double getHeight();
	int getCountNum();
	bool getAutoState();

	void setTrackLength(int);
	void setHeight(double);
	void setCountNum(int);
	void setAutoState(bool);

signals:
	void signalClear();
	void signalAccept();
	void signalCancel();
	void signalAuto(bool);
};

#endif // SOLVERSETTINGSDIALOG_H
