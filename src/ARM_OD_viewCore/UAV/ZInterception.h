#ifndef ZINTERCEPTION_H
#define ZINTERCEPTION_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QTime>
#include <QDebug>
#include <qmath.h>

#include "Map/IMapClient.h"

typedef struct tt
{
	QPointF *point;
	double alt;
	double speed;
	double course;
	int state;
}tt;

/*!
   Класс, который определяет координаты точки перехвата
*/
class ZInterception : public QObject
{
	Q_OBJECT
public:
	explicit ZInterception(IMapClient *client);
	~ZInterception();

public:
	void set(int bla, int bpla, QByteArray per, QByteArray target);

signals:
	/*!
		Сигнал для отправки данных о точке перехвата.
		\param aCoord - координаты точки перехвата.
		\param aHgt - высота точки перехвата.
		\param aRadius - радиус зоны перехвата.
		\param aTime - время перехвата в секундах.
		\param aIntcCourse - курс, необходимый для перехватчика.
		\param aIntcSpeed - скорость, необходимая для перехватчика.
	*/
	void signal_sendInterceptionPoint(QPointF aCoord, float aHgt, float aRadius, int aTime, float aIntcCourse, float aIntcSpeed);
	void finished();

	void signalSet(int bla, int bpla, QByteArray per, QByteArray target);

private slots:
	void _slot_set(int bla, int bpla, QByteArray per, QByteArray target);

public slots:
	/*!
		Слот получения данных о перехватчике.
		\param aCoordCurrentIntc - координаты перехватчика.
		\param aHgtCurrentIntc - высота перехватчика.
		\param aMaxSpd - максимальная скорость перехватчика.
		\param aMaxHgtSpd - максимальная скорость набора высоты перехватчика.
	*/
	void getIntcData(QPointF aCoordCurrentIntc, float aHgtCurrentIntc, float aMaxSpd, float aMaxHgtSpd);

	/*!
		Слот получения данных о перехватчике.
		\param aCoordCurrentAim - координаты цели.
		\param aHgtCurrentAim - высота цели.
		\param aSpdAim - скорость цели.
		\param aCourseAim - курс цели.
	*/
	void getAimData(QPointF aCoordCurrentAim, float aHgtCurrentAim, float aSpdAim, float aCourseAim);

	/*!
		Основная процедура вычисления.
		\param aDecartCoord - декартовы координаты.
		\param lGeogrCoord - ccылка на географические координаты.
		\param aCentrCoord - географические координаты центральной точки, которая соответсвует точке в декартовых координатах (0,0).
	*/
	void mainProcessing();

private:
	/*!
		Перевод географических координат в декартовы (для 1 точки).
		\param aGeogrCoord - географические координаты.
		\param lDecartCoord - ccылка на декартовы координаты.
		\param aCentrCoord - географические координаты центрально точки, которая соответсвует точке в декартовых координатах (0,0).
	*/
	void geogrToDecart(QPointF aGeogrCoord,QPointF &lDecartCoord, QPointF aCentrCoord);

	/*!
		Перевод декартовых координат в географические (для 1 точки).
		\param aDecartCoord - декартовы координаты.
		\param lGeogrCoord - ccылка на географические координаты.
		\param aCentrCoord - географические координаты центральной точки, которая соответсвует точке в декартовых координатах (0,0).
	*/
	void decartToGeogr(QPointF aDecartCoord, QPointF &lGeogrCoord, QPointF aCentrCoord);

	//! � асчет расстояния между 2мя точками (декарт.)
	float getDistance(QPointF aDecartCoordA, QPointF aDecartCoordB);

	/*!
			Определение курсового угла и скорости движения по XY.
			\param aDecartCoord - декартовы координаты.
			\param aDecartCoordB - декартовы координаты.
			\param aSpeed - скорость.
			\param lXYSpeed - ссылка на скорость по XY.
			\param lAngle - ссылка на курсовой угол.
	*/
	void getAngle(QPointF aDecartCoordA, QPointF aDecartCoordB, float aSpeed, QPointF &lXYSpeed, float &lAngle);

	/*!
			Определение декартовых координат цели в след. момент времени.
			\param aDecartCoordAim - начальные декартовы координаты.
			\param aAngle - курсовой угол.
			\param aSpeed - скорость.
			\param aTimeSec - время в секундах, по которому нужно рассчитать новые координаты.
			\param lDecartCoord - ссылка на новые декартовы координаты.
	*/
	void getNextCoord(QPointF aDecartCoordAim, float aAngle, float aSpeed, int aTimeSec, QPointF &lDecartCoord);




private:
	//! Данные о перехватчике
	QPointF mCoordCurrentIntc;      //Текущие координаты
	float mHgtCurrentIntc;          //Текущая высота
	float mMaxSpd;                    //Максимально возможная скорость
	float mCurrentSpd;              //Текущая скорость
	float mMaxHgtSpd;                 //Максимальная скорость набора высоты
	float mCurrentCourse;           //Текущий курс

	//! Данные о цели
	QPointF mCoordCurrentAim;
	float mHgtCurrentAim;
	float mCurrentAimSpd;
	float mCurrentAimCourse;

	IMapClient*     _client;

	int _bla;
	int _bpla;

};

#endif // ZINTERCEPTION_H
