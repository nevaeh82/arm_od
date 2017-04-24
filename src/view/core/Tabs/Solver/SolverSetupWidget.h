#pragma once

#include <QWidget>
#include <QDialog>
#include <QByteArray>
#include <QScrollBar>
#include <QTableWidget>

#include "SolverPacket1.pb.h"

namespace Ui {
class SolverSetupWidget;
}

class SolverSetupWidget : public QDialog
{
	Q_OBJECT

public:
	explicit SolverSetupWidget(QWidget *parent = 0);
	~SolverSetupWidget();

	void setSolverVersion(const QString &version);
	void setSolverSettings(SolverProtocol::Packet_DataFromSolver_SolverResponse &data);

	SolverProtocol::Packet getSolverParams();

private:
	Ui::SolverSetupWidget *ui;

	void addTableItem(QTableWidget* widget, int r, int c, QVariant value, bool isNum, bool isGrad = false);
	void addTableCheckbox(QTableWidget* widget, int r, int c, const QString &title);
	void ClearTableItem(QTableWidget* widget);
	int m_workAreaSetCounter;
	int m_solverMessageCounter;

public:
	void appendSolverResult(const QString& log);

	SolverProtocol::Packet_DataFromClient_SetRequest_SetClientType getClientParams();
	void setSolutionConfiguration(SolverProtocol::Packet_DataFromClient_SetRequest *sPkt);

	void setMapClick(double lon, double lat );

	void setSolverMessage(int, QString);
private slots:
	void slotAddStation();
	void slotRemoveStation();

signals:
	void onSendSignal();
	void onGetVersion();

	void signalStopSolver();
	void signalRestartSolver();
	void signalClearSolver();
	void signalStartSolverCorrections();
	void signalStopSolverCorrections();
	void signalResetSolverCorrections();
	void signalSaveSolverCorrections();

	void signalSetUser();
	void signalGetAll();

	void signalSetParams();

	void onCorrectionCalculation(int);
};
