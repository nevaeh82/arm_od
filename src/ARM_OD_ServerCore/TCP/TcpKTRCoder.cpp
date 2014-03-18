#include "TcpKTRCoder.h"

TcpKTRCoder::TcpKTRCoder(QObject *parent) :
	BaseTcpDeviceCoder(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpKTRCoder)), parent)
{
}

TcpKTRCoder::~TcpKTRCoder()
{
}

MessageSP TcpKTRCoder::encode(const QByteArray& data)
{
	m_logger->debug(QString("Parse data from KTR %1").arg(QString::fromAscii(data.constData(), data.size())));

	QByteArray inputData = data;
	inputData.remove(0, 12);

	/*
	int count_bla = inputData.at(0);
	QVector<quint16> identificators;

	for (int i = 0; i < count_bla; i++) {
		quint16 id = ((quint8)inputData.at(3 + i*24) << 8) + (quint8)inputData.at(2 + i*24);
		identificators.push_back(id);
		m_logger->debug(QString("ID = %1").arg(QString::number(id)));
	}
	*/
	/*
	QList<quint16> boardsList;

	QDataStream stream(inputData);
	stream >> boardsList;

	m_logger->debug(QString("Boards count = %1").arg(boardsList.size()));
	foreach (quint16 board, boardsList) {
		m_logger->debug(QString("ID = %1").arg(QString::number(board)));
	}
	*/
	return MessageSP(new MessageSP<QByteArray>(TCP_KTR_ANSWER_BOARD_LIST, inputData));
}

QByteArray TcpKTRCoder::decode(const MessageSP message)
{
	/**
	 * TCP_KTR_REQUEST_GET_BOARD_LIST
	 * "user link command ctrl" as Ascii in QByteArray
	 **/

	return message->data();
}

QObject* TcpKTRCoder::asQObject()
{
	return this;
}
