#include "SolverSetupWidgetController.h"

#include "SolverPacket1.pb.h"
#include "SolverExchange.h"

#include "QMessageBox"

#define SOLVERSETTINGS_REQUEST_TIMEOUT 5000

SolverSetupWidgetController::SolverSetupWidgetController(ITabManager* tabManager, QObject* parent):
	QObject(parent),
	m_isSend(false),
	m_isReceivedConfig(false)
{
	m_view = NULL;
	m_tabManager = tabManager;

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));

}

SolverSetupWidgetController::~SolverSetupWidgetController()
{
	if(m_view != NULL)
	{
		delete m_view;
	}
}

void SolverSetupWidgetController::appendView(SolverSetupWidget *view)
{
	m_view = view;

	connect(m_view, SIGNAL(onSendSignal()), this, SLOT(slotSendSettings()));
	connect(m_view, SIGNAL(onGetVersion()), this, SLOT( slotGetVersion()) );

	connect(m_view, SIGNAL(signalStopSolver()), this, SLOT(slotSendCommandStop()));
	connect(m_view, SIGNAL(signalRestartSolver()), this, SLOT(slotSendCommandRestart()));
	connect(m_view, SIGNAL(signalClearSolver()), this, SLOT(slotSendCommandClear()));

	connect(m_view, SIGNAL(onCorrectionCalculation(int)), this, SLOT(slotCorrectionCalculation(int)));
	connect(m_view, SIGNAL(signalStartSolverCorrections()), this, SLOT(slotSendCommandStartCor()));
	connect(m_view, SIGNAL(signalStopSolverCorrections()), this, SLOT(slotSendCommandStopCor()));

	connect(m_view, SIGNAL(signalResetSolverCorrections()), this, SLOT(slotSendCommandResetCor()));
	connect(m_view, SIGNAL(signalSaveSolverCorrections()), this, SLOT(slotSendCommandSaveCor()));

	connect(m_view, SIGNAL(signalSetUser()), this, SLOT(slotSendUserData()));
	connect(m_view, SIGNAL(signalGetAll()), this, SLOT(slotGetAll()));

	connect(m_view, SIGNAL(signalSetParams()), this, SLOT(slotSetParams()));

}

void SolverSetupWidgetController::onMethodCalled(const QString &method, const QVariant &argument)
{
	emit onMethodCalledSignal(method, argument);
}

SolverSetupWidget *SolverSetupWidgetController::getView()
{
	return m_view;
}

void SolverSetupWidgetController::setMapFlag()
{
	m_isReceivedConfig = false;
	m_startTimer.singleShot( SOLVERSETTINGS_REQUEST_TIMEOUT, this, SLOT(slotRequestConfig()) );
}

void SolverSetupWidgetController::setARMRConnection(bool b)
{
	if(b) {
		m_isReceivedConfig = false;
		slotGetAll();
	}
}

void SolverSetupWidgetController::slotShowWidget()
{
	if(m_view != NULL)
	{
		m_view->show();
	}
}

void SolverSetupWidgetController::onMethodCalledSlot(QString method, QVariant argument)
{
	if (method == RPC_SLOT_SERVER_SEND_BPLA_POINTS_1) {
		//debug
		int i = 0;

		QByteArray data = argument.toByteArray();
		SolverProtocol::Packet pkt;
		pkt.ParseFromArray( data.data(), data.size() );

		if( isSolverMessageCommandSolver(pkt) && m_isSend) {
			m_isSend = false;
			QString msg = QString::fromStdString(pkt.datafromsolver().message().message());
			//QMessageBox::information( m_view, tr("SolverMessage"), msg, QMessageBox::Yes );
		}

		if( isSolverMessageSolverResponse( pkt ) ) {
			SolverProtocol::Packet_DataFromSolver_SolverResponse response = pkt.datafromsolver().solverresponse();
			m_view->setSolverSettings( response );
			if(response.has_detectors()) {
				m_isReceivedConfig = true;
			}
		}

		if( isSolverMessageSolverMessage(pkt) ) {
			m_view->setSolverMessage( (int)pkt.datafromsolver().message().messagetype(),
									  QString::fromStdString(pkt.datafromsolver().message().message()));
		}

	} else if(method == RPC_SLOT_SERVER_SEND_ARMR_CONNECTION) {
		bool b = argument.toBool();

		setARMRConnection(b);

		if(m_tabManager) {
			m_tabManager->set_arm_r_connection(b);
		}
	}
}

void SolverSetupWidgetController::slotSendSettings()
{
	emit onSendSolverCommandSettings(QByteArray());
}

void SolverSetupWidgetController::slotGetVersion()
{
	SolverProtocol::Packet pkt;
	pkt.mutable_datafromclient()->add_getrequest( SolverProtocol::Packet_DataFromClient_GetRequest_getSolverVersion );

	QByteArray data;
	data.resize( pkt.ByteSize() );
	pkt.SerializeToArray(data.data(), data.size());

	addPreambula( data );

	emit onSendSolverCommandSettings( data );
}

void SolverSetupWidgetController::addPreambula(QByteArray& data)
{
	uint size = data.size();
	data.prepend((char*)&size, sizeof(unsigned int));
	data.prepend("SolverPacket");
}

//SendCommands
void SolverSetupWidgetController::slotSendCommandRestart()
{
	SolverProtocol::Packet pkt;
	createCommandSolverRestartServer( pkt );

	sendSolverCommand(pkt);
}

void SolverSetupWidgetController::slotSendCommandClear()
{
	SolverProtocol::Packet pkt;
	createCommandSolverClear( pkt );

	sendSolverCommand(pkt);
}

void SolverSetupWidgetController::slotSendCommandStop()
{
	SolverProtocol::Packet pkt;
	createCommandSolverStopServer( pkt );

	sendSolverCommand(pkt);
}

void SolverSetupWidgetController::slotSendCommandStartCor()
{
	SolverProtocol::Packet pkt;
	createCommandSolverStartCalcCorrections( pkt );

	sendSolverCommand(pkt);
}

void SolverSetupWidgetController::slotSendCommandStopCor()
{
	SolverProtocol::Packet pkt;
	createCommandSolverStopCalcCorrections( pkt );

	sendSolverCommand(pkt);
}

void SolverSetupWidgetController::slotCorrectionCalculation(int isCorrect)
{
	if(isCorrect) {
		slotSendCommandStartCor();
	} else {
		slotSendCommandStopCor();
	}
}


void SolverSetupWidgetController::slotSendCommandResetCor()
{
	SolverProtocol::Packet pkt;
	createCommandSolverResetCalcCorrections( pkt );

	sendSolverCommand(pkt);
}

void SolverSetupWidgetController::slotSendCommandSaveCor()
{
	SolverProtocol::Packet pkt;
	createCommandSolverSaveCalcCorrections( pkt );

	sendSolverCommand(pkt);
}

void SolverSetupWidgetController::slotSendUserData()
{
	SolverProtocol::Packet pkt;
	createCommandSolverSetUser( pkt, m_view->getClientParams() );

	sendSolverCommand(pkt);
}

void SolverSetupWidgetController::slotGetAll()
{
	SolverProtocol::Packet pkt;
	createCommandSolverGetAll( pkt );

	sendSolverCommand(pkt);
}

void SolverSetupWidgetController::slotSetParams()
{
	SolverProtocol::Packet pkt;
	pkt = m_view->getSolverParams();

	sendSolverCommand(pkt);
}

void SolverSetupWidgetController::slotRequestConfig()
{
	if(!m_isReceivedConfig) {
		m_startTimer.singleShot( SOLVERSETTINGS_REQUEST_TIMEOUT, this, SLOT(slotRequestConfig()) );
		slotGetAll();
	}
}

void SolverSetupWidgetController::slotMapClicked(double lon, double lat)
{
	m_view->setMapClick( lon, lat );
}

void SolverSetupWidgetController::sendSolverCommand( const SolverProtocol::Packet& pkt )
{
	QByteArray ba;
	ba.resize( pkt.ByteSize() );
	pkt.SerializeToArray( ba.data(), ba.size() );

	CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER, ba);
	m_tabManager->send_data(0, msg);

	m_isSend = true;
}
