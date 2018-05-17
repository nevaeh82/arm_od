#ifndef GEORECTANGLE_H
#define GEORECTANGLE_H
#include <QObject>
#include "PwGisClient.h"

#define EPSILON 0.0001

class GISCLIENT_EXPORT GeoRectangle : public QObject
{

	Q_OBJECT
	Q_CLASSINFO("ClassID","{ca87894d-96b0-445e-b0a9-47461b8083b0}")
	Q_CLASSINFO("InterfaceID","{60d460cb-48f2-470f-ba05-9d5c720a46dc}")


private:
	/// min horizontal coordinate.
	double _left;

	/// min vertical coordinate.
	double _bottom;

	/// max horizonal coordinate.
	double _right;

	/// max vertical coordinate.
	double _top;

	bool areEqual(double a, double b);

public:
	GeoRectangle(double left = 0 , double bottom = 0 , double right = 0 , double top = 0 , QObject* parent = 0);

	GeoRectangle(GeoRectangle& gr);

	GeoRectangle& operator=(const GeoRectangle& gr);

	bool operator==(const GeoRectangle& gr);

public slots:
	double getLeft() const { return _left; }
	double getBottom() const { return _bottom; }
	double getRight() const { return _right; }
	double getTop() const { return _top; }

	void setLeft(double left) { _left = left; }
	void setBottom(double bottom) { _bottom = bottom; }
	void setRight(double right) { _right = right; }
	void setTop(double top) { _top = top; }

};

#endif // GEORECTANGLE_H
