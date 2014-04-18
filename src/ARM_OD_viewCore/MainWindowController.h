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

#include "Tabs/RPC/RpcConfigClient.h"
#include "Rpc/RpcDefines.h"

class MainWindowController : public QObject, public IRpcListener//, public IController<MainWindow>
{
	Q_OBJECT

private:

	MainWindow* m_view;

	Pw::Common::ServiceControl::ServiceHandler* m_serverHandler;

	TabManager*  m_tabManager;

	RpcConfigClient* m_rpcConfigClient;

	SolverSettingsDialogController* solverSettingsController;

	int m_uavLifeTime;

public:
	explicit MainWindowController(QObject *parent = 0);
	virtual ~MainWindowController();

	void appendView(MainWindow *view);

	void startServer();

	void setUavLifeTime(int msecs);

signals:

private slots:
	void serverFailedToStartSlot();
	void serverStartedSlot();

	void startTabManger();

	void rpcConnectionEstablished();

	void solverDialogSlot();

private:

	void start();
	void stop();

	void init();

	// IRpcListener interface
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);
};

#endif // MAINWINDOWCONTROLLER_H
