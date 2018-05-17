#ifndef SPACEMODEL_H
#define SPACEMODEL_H

#include <vector>


struct SpaceModel {
	typedef std::vector<double> Vector;

	static SpaceModel::Vector toVector
	(double altitude, double sinLat, double cosLat, double sinLon, double cosLon);

	static double getLatitude(SpaceModel::Vector const &vector);
	static double getLongitude(SpaceModel::Vector const &vector);

	static Vector rotate(Vector const &vector, Vector const &axis, double angle);

private:
	typedef std::vector<Vector> Basis;

	static Vector rotateZ(Vector const &vector, double angle);

	static Basis transform(Basis const &a, Basis const &b);
	static Vector transform(Vector const &vector, Basis const &basis);

	static double project(Vector const &a, Vector const &b);

	static Vector normalize(Vector const &vector);
	static double getLength(Vector const &vector);

	static Vector mul(Vector const &a, Vector const &b);
	static double addSquare(double a, double b);

	static Vector createVector(double x, double y, double z);
	static Basis createBasis(Vector const &i, Vector const &j, Vector const &k);

	template <class T> static
	std::vector<T> create(T const &item1, T const &item2, T const &item3);
};

#endif // SPACEMODEL_H
