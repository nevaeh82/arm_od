#ifndef KTRCODERTEST_H
#define KTRCODERTEST_H

#include <cxxtest/TestSuite.h>
#include <QDebug>

#include "TCP/TcpKTRCoder.h"

class KtrCoderTest : public CxxTest::TestSuite
{
public:

	void testKtrParser()
	{
		QString inputString("$KTPGA,OBJ_ID=10187,3X,P0={42*28'51.19\"|41*33'45.65\"},END\r\n");

		TcpKTRCoder* coder =  new TcpKTRCoder;
		MessageSP msg = coder->parseLocationFromKTR(QByteArray(inputString.toAscii()));

		TS_ASSERT_EQUALS(TCP_KTR_ANSWER_BPLA, msg.data()->type());
		TS_ASSERT_EQUALS(false, msg.data()->data().isEmpty());

		QDataStream inputDataStream(&(msg.data()->data()), QIODevice::ReadOnly);
		QVector<UAVPositionData> positionDataVector;
		inputDataStream >> positionDataVector;

		TS_ASSERT_EQUALS(1, positionDataVector.size());
		TS_ASSERT_DELTA( 42.4808, positionDataVector.at(0).latitude, 0.0001);
		TS_ASSERT_DELTA( 41.5626, positionDataVector.at(0).longitude, 0.0001);

		/// FOR FUTURE, DO NOT DELETE!

//		inputString = "$KTPGA,OBJ_ID=10187,3X,P0={42*28'51.19\"|41*33'45.65\"},P1={43*28'51.19\"|42*33'45.65\"},END\r\n";

//		msg = coder->parseLocationFromKTR(QByteArray(inputString.toAscii()));

//		TS_ASSERT_EQUALS(TCP_KTR_ANSWER_BPLA, msg.data()->type());
//		TS_ASSERT_EQUALS(false, msg.data()->data().isEmpty());

//		QDataStream inputDataStream2(&(msg.data()->data()), QIODevice::ReadOnly);
//		QVector<UAVPositionData> positionDataVector2;
//		inputDataStream2 >> positionDataVector2;

//		TS_ASSERT_EQUALS(2, positionDataVector2.size());
//		TS_ASSERT_DELTA( 42.4808, positionDataVector2.at(0).latitude, 0.0001);
//		TS_ASSERT_DELTA( 41.5626, positionDataVector2.at(0).longitude, 0.0001);

//		TS_ASSERT_DELTA( 43.4808, positionDataVector2.at(1).latitude, 0.0001);
//		TS_ASSERT_DELTA( 42.5626, positionDataVector2.at(1).longitude, 0.0001);


		delete coder;
	}
};



#endif // KTRCODERTEST_H
