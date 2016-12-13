#pragma once

#include <QObject>
#include <QVariant>
#include <QTimer>

#include <Rpc/RpcDefines.h>

#include "Interfaces/IController.h"
#include "../ITabManager.h"

#include "SolverSetupWidget.h"
#include "Interfaces/IRpcListener.h"

#include "Common/CommandMessage.h"

#include "SolverPacket1.pb.h"

class SolverSetupWidgetController : public QObject, public IController<SolverSetupWidget>,
									 public IRpcListener
{
	Q_OBJECT
public:
	explicit SolverSetupWidgetController(ITabManager* tabManager, QObject *parent = 0);
	virtual ~SolverSetupWidgetController();


private:
	SolverSetupWidget* m_view;
	ITabManager* m_tabManager;

	void addPreambula(QByteArray &data);
	void sendSolverCommand(const SolverProtocol::Packet& data);

	bool m_isSend;
    bool m_isReceivedConfig;

    QTimer m_startTimer;
public:
	void appendView(SolverSetupWidget* view);
	virtual void onMethodCalled(const QString& method, const QVariant& argument);
	SolverSetupWidget* getView();

    void setMapFlag();

public slots:
	void slotShowWidget();
	void slotGetAll();


signals:
	void onMethodCalledSignal(QString, QVariant);

	void onSendSolverCommandSettings(QByteArray data);

private slots:
	void onMethodCalledSlot(QString method, QVariant data);

	void slotSendSettings();
	void slotGetVersion();

	void slotSendCommandStop();
	void slotSendCommandRestart();
	void slotSendCommandClear();
	void slotSendCommandStartCor();
	void slotSendCommandStopCor();
	void slotSendCommandResetCor();
	void slotSendCommandSaveCor();
	void slotSendUserData();

    void slotSetParams();

    void slotRequestConfig();

	void slotMapClicked(double lon, double lat);
};

