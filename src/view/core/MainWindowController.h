#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>
#include <QEventLoop>

#include <SettingsManager/RpcSettingsManager.h>

//#include "Interfaces/IController.h"
#include "MainWindow.h"

#include "ServiceHandler/ServiceHandler.h"

#include "Tabs/TabManager.h"
#include "Tabs/Solver/SolverSettingsDialogController.h"
#include "Tabs/Solver/SolverSettingsDialog.h"

#include "Tabs/Solver/SolverSetupWidget.h"
#include "Tabs/Solver/SolverSetupWidgetController.h"

#include "Tabs/RPC/RpcConfigClient.h"
#include "Rpc/RpcDefines.h"

#include "BaseParser/BaseParser.h"

#define SERVER_NAME "ZaviruhaODServer"
#define GUI_NAME "ZaviruhaODView"

class MainWindowController : public QObject, public IRpcListener//, public IController<MainWindow>
{
	Q_OBJECT

private:

	MainWindow* m_view;

	TabManager*  m_tabManager;

	RpcConfigClient* m_rpcConfigClient;

	SolverSettingsDialogController* solverSettingsController;
	SkyHobbit::Common::ServiceControl::ServiceHandler* m_serverHandler;

	SolverSetupWidget* m_solverSetupWgt;
	SolverSetupWidgetController* m_solverSetupWidgetController;

	int m_uavLifeTime;

	bool m_ping;
	bool m_armrConnection;

	QTimer* m_rpcCheckTimer;

public:
	explicit MainWindowController(QObject *parent = 0);
	virtual ~MainWindowController();

	void appendView(MainWindow *view);

	void setUavLifeTime(int msecs);

private slots:
	void serverFailedToStartSlot();
	void serverStartedSlot();
	void resetServer();

	void startTabManger();

	void rpcConnectionEstablished();

	void solverDialogSlot();

	void enableAdsbClient(bool);
	void enableAdsbOnlineClient(bool);

	void mapOpened();

	void slotServerProcessDead();
	void slotCheckRpc();

private:

	void init();

	// IRpcListener interface
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);
};

#endif // MAINWINDOWCONTROLLER_H
