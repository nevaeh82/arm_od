#include "TcpNIIPPCoder.h"

#include "SolverPacket1.pb.h"

TcpNIIPPCoder::TcpNIIPPCoder(int id, QObject* parent) :
	BaseTcpDeviceCoder(parent)
{
    m_id = id;
}

TcpNIIPPCoder::~TcpNIIPPCoder()
{
}

MessageSP TcpNIIPPCoder::encode(const QByteArray& data)
{
	QString str(data);
	QStringList list = str.split(',');

    QString dse = list[0];
    QDate date;
    date = QDate::fromString(list[0], "ddMMyy");

	QTime time;
    time = QTime::fromString(list[1], "HHmmss");

    QDateTime dt;
    dt.setDate(date);
    dt.setTime(time);

	int mode = list[2].toInt();

	QString lat_str = list[3];
	QString lat_sec_str = lat_str.right(2);

	int sec_int = lat_sec_str.toInt();
	double sec = (double)sec_int/3600;

	QString lat_min_str_temp = lat_str.right(4);
	QString lat_min_str = lat_min_str_temp.left(2);

	int min_int = lat_min_str.toInt();
	double min = (double)min_int/60;

	QString lat_grad = lat_str.left(lat_str.length() - 4);

	int grad_int = lat_grad.toInt();
	double grad = (double)grad_int + min + sec;

	QPointF point;
	point.setX(grad);

	QString NS = list[4];

	QString lon_str = list[5];
	QString lon_sec_str = lon_str.right(2);
	sec_int = lon_sec_str.toInt();
	sec = (double)sec_int/3600;

	QString lon_min_str_temp = lon_str.right(4);
	QString lon_min_str = lon_min_str_temp.left(2);
	min_int = lon_min_str.toInt();
	min = (double)min_int/60;

	QString lon_grad = lon_str.left(lon_str.length() - 4);
	grad_int = lon_grad.toInt();
	grad = (double)grad_int + min + sec;
	point.setY(grad);

	QString EW = list[6];

	int alt = list[7].toInt();
	int zone = list[8].toInt();
	int course = list[9].toInt();
	int angle = list[10].toInt();

	QByteArray dataToSend;
	QDataStream ds(&dataToSend, QIODevice::ReadWrite);

	/// TODO : recheck _id
    ds << m_id;//	ds << _id;

	ds << dt;
	ds << time;
	ds << mode;
	ds << point;
	ds << NS;
	ds << EW;
	ds << alt;
	ds << zone;
	ds << course;
	ds << angle;

    //log_debug(QString("MMMMMMMMMMMMMMMMMMMMMM %1 %2 %3").arg(course).arg(angle).arg(mode));
	return MessageSP(new Message<QByteArray>(TCP_NIIPP_ANSWER, dataToSend));

//	QSharedPointer<IMessageOld> msg(new MessageOld(_id, NIIPP_ANSWER, ba));
//	_subscriber->data_ready(NIIPP_ANSWER, msg);
//	qDebug() << "MMMMMMMMMMMMMMMMMMMMMM" << course << angle << mode;

}

QByteArray TcpNIIPPCoder::decode(const MessageSP message)
{
	if (message->type() != TCP_NIIPP_REQUEST_PBLA) {
		return QByteArray();
	}

    QByteArray data = message->data();

    SolverProtocol::Packet solutionPacket;

    if( !solutionPacket.ParseFromArray(data.data(), data.size()) ) {
        return QByteArray();
    }

    SolverProtocol::Packet_DataFromSolver_SolverSolution dPkt;
    if(solutionPacket.datafromsolver().has_solution_automatic_altitude()) {
        dPkt = solutionPacket.datafromsolver().solution_automatic_altitude();
    } else if(solutionPacket.datafromsolver().has_solution_manual_altitude()) {
        dPkt = solutionPacket.datafromsolver().solution_manual_altitude();
    } else {
        return QByteArray();
    }

    QString all;
    for(int i = 0; i<dPkt.trajectory_size(); i++) {
        QString message;

        SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory tPkt =  dPkt.trajectory( i );

        SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory_MotionEstimate mPkt =
                tPkt.motionestimate(tPkt.motionestimate_size()-1);

        QStringList niippLst;
        niippLst.append("BPLA");
        niippLst.append(QString::fromStdString(tPkt.targetid()));

        quint64 t = mPkt.datetime();
        QDateTime dt = QDateTime::fromMSecsSinceEpoch(t);

        niippLst.append( dt.date().toString("ddMMyy") );
        niippLst.append( dt.time().toString("hhmmss") );

        niippLst.append( QString::number(mPkt.coordinates().alt()) );
        niippLst.append( QString::number(mPkt.targetspeed()) );
        niippLst.append( QString::number(mPkt.relativebearing()) );
        niippLst.append( QString("+") + QString::number(mPkt.coordinates().lat()) );
        niippLst.append( QString("+") + QString::number(mPkt.coordinates().lon()) );

        double acc = sqrt( mPkt.coordinates_acc().lon_acc()*mPkt.coordinates_acc().lon_acc() +
                           mPkt.coordinates_acc().lat_acc()*mPkt.coordinates_acc().lat_acc() );

        niippLst.append( QString::number(acc) );
        message = niippLst.join(",");

        quint8 n =  crc( message.toLocal8Bit() );

        niippLst.append( "*" );
        message.append(QString::number(n));

        message.append("\\r\\n");
        all.append( message );
    }

    return all.toLocal8Bit();
}

QByteArray TcpNIIPPCoder::prepareDataToSend(const QStringList& list)
{
	QString str = list.join(",");
	quint8 _crc = crc(str.toLocal8Bit());

	str.append(",*");
	str.append(QString::number(_crc));
	str.append("\\r\\n");

	return str.toLocal8Bit();
}

quint8 TcpNIIPPCoder::crc(const QByteArray& data)
{
	quint8 crc = 0;
	for (int i = 0; i < data.length(); i++) {
		crc ^= data.at(i);
	}
	return crc;
}
