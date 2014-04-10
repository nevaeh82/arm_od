#include "RpcConfigReader.h"

RpcConfigReader::RpcConfigReader(QObject* parent) :
	QObject(parent)
{
	m_rpcSettingsManager = RpcSettingsManager::instance();
	m_dbUavSettingsManager = DbBlaSettingsManager::instance();
	connect(this, SIGNAL(readStationListInternalSignal(QString)), this, SLOT(readStationListInternalSlot(QString)));
	connect(this, SIGNAL(readDbConfigurationInternalSignal(QString)), this, SLOT(readDbConfigurationInternalSlot(QString)));
}

RpcConfigReader::~RpcConfigReader()
{
}

void RpcConfigReader::setRpcServer(IRpcControllerBase* rpcServer)
{
	m_rpcServer = rpcServer;
	m_rpcServer->registerReceiver(this);
}

void RpcConfigReader::onMethodCalled(const QString& method, const QVariant& argument)
{
	QString fileName = argument.toString();

	if (method == RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST) {
		emit readStationListInternalSignal(fileName);
	} else if (method == RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION) {
		emit readDbConfigurationInternalSignal(fileName);
	}
}

void RpcConfigReader::readStationListInternalSlot(const QString& fileName)
{
	QSettings stationSettings(fileName, QSettings::IniFormat, this);
	stationSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	QList<StationConfiguration> stationList;

	QStringList childKeys = stationSettings.childGroups();

	foreach (const QString &childKey, childKeys ) {

		StationConfiguration stationConfiguration;

		stationSettings.beginGroup(childKey);
		stationConfiguration.id				= stationSettings.value("Id", 0).toUInt();
		stationConfiguration.name			= stationSettings.value("Name", "Unknown").toString();
		stationConfiguration.latitude		= stationSettings.value("Latitude", 0.0).toDouble();
		stationConfiguration.longitude		= stationSettings.value("Longitude", 0.0).toDouble();
		stationSettings.endGroup();

		stationList.append(stationConfiguration);
	}

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << stationList;

	m_rpcServer->call(RPC_METHOD_CONFIG_ANSWER_STATION_LIST, QVariant(dataToSend));
}

void RpcConfigReader::readDbConfigurationInternalSlot(const QString& filename)
{
	m_dbUavSettingsManager->setIniFile(filename);
	DBConnectionStruct param;

	param.host		= m_dbUavSettingsManager->getBlaDbHost();
	param.port		= m_dbUavSettingsManager->getBlaDbPort();
	param.login		= m_dbUavSettingsManager->getBlaDbLogin();
	param.password	= m_dbUavSettingsManager->getBlaDbPassword();
	param.dbName	= m_dbUavSettingsManager->getBlaDbName();

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << param;

	m_rpcServer->call(RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION, QVariant(dataToSend));
}
