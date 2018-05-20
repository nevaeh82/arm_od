#include "SpaceModel.h"

#include <cmath>
#include <numeric>
#include <QtCore/qmath.h>


SpaceModel::Vector SpaceModel::toVector(double altitude, double sinLat, double cosLat, double sinLon, double cosLon) {
	/* latitude scale is: [N]0 .. [E]+90 .. [S]+180:
	 *   x = altitude * sinLat * cosLon
	 *   y = altitude * sinLat * sinLon
	 *   z = altitude * cosLat
	 *
	 * but geo latitude scale is: [N]+90 .. [E]0 .. [S]-90:
	 *   lat = PI/2 - latitude then
	 *   sinLat => cosLat
	 *   cosLat => sinLat
	 */

	return createVector(
		 altitude * cosLat * cosLon,
		 altitude * cosLat * sinLon,
		 altitude * sinLat
	);
}

double SpaceModel::getLatitude(SpaceModel::Vector const &vector) {
	double const length = getLength(vector);
	if (length == 0) return 0;
	double pi = qAcos( -1.0 );

	// geo latitude scale is: [N]+90 .. [E]0 .. [S]-90
	return pi/2 - acos(vector[2] / length);
}

double SpaceModel::getLongitude(SpaceModel::Vector const &vector) {
	double const &x = vector[0];
	double const &y = vector[1];
	double pi = qAcos( -1.0 );

	if (x == 0) {
		if (y == 0) return 0;
		return y > 0 ? pi : - pi;
	}

	double const a = atan(vector[1]/vector[0]);
	if (x > 0) return a;

	return y > 0 ? a + pi : a - pi;
}


SpaceModel::Vector SpaceModel::rotate(Vector const &vector, Vector const &axis, double angle) {
	static Basis const basis = createBasis(
		createVector(1, 0, 0),
		createVector(0, 1, 0),
		createVector(0, 0, 1)
	);

	Vector const k = normalize(axis);
	Vector const i = normalize(mul(vector, k));
	Basis const tempBasis = createBasis(i, mul(i, k), k);

	return transform(
		rotateZ(transform(vector, tempBasis), angle),
		transform(basis, tempBasis)
	);
}

SpaceModel::Vector SpaceModel::rotateZ(Vector const &vector, double angle) {
	double const sinA = sin(angle);
	double const cosA = cos(angle);

	/* Z-rotation Matrix X Vector
	 *  + cosA * vector[0] + sinA * vector[1] + 0 * vector[2]
	 *  - sinA * vector[0] + cosA * vector[1] + 0 * vector[2]
	 *  +   0  * vector[0] +   0  * vector[1] + 1 * vector[2]
	 */

	return createVector(
		+ vector[0] * cosA + vector[1] * sinA,
		- vector[0] * sinA + vector[1] * cosA,
		+ vector[2]
	);
}


SpaceModel::Basis SpaceModel::transform(Basis const &a, Basis const &b) {
	Vector::size_type const size = a.size();
	Basis result(size);

	for (Vector::size_type i = 0; i < size; i++)
		result[i] = transform(a[i], b);

	return result;
}

SpaceModel::Vector SpaceModel::transform(Vector const &vector, Basis const &basis) {
	Vector::size_type const size = basis.size();
	Vector result(size);

	for (Basis::size_type i = 0; i < size; i++)
		result[i] = project(vector, basis[i]);

	return result;
}


double SpaceModel::project(Vector const &a, Vector const &b) {
	double const length = getLength(b);
	if (length == 0) return 0;

	Vector::size_type const size = a.size();

	double tmp = 0;

	for (Vector::size_type i = 0; i < size; i++)
		tmp += a[i] * b[i];

	return tmp / length;
}



SpaceModel::Vector SpaceModel::normalize(Vector const &vector) {
	double const length = getLength(vector);
	if (length == 0) return vector;

	Vector::size_type const size = vector.size();

	Vector result(size);

	for (Vector::size_type i = 0; i < size; i++)
		result[i] = vector[i] / length;

	return result;
}

double SpaceModel::getLength(Vector const &vector) {
	double const sum = std::accumulate(vector.begin(), vector.end(), double(0), &addSquare);
	return sqrt(sum);
}


SpaceModel::Vector SpaceModel::mul(Vector const &a, Vector const &b) {
	return createVector(a[2]*b[1] - a[1]*b[2], a[0]*b[2] - a[2]*b[0], a[1]*b[0] - a[0]*b[1]);
}

double SpaceModel::addSquare(double a, double b) {
	return a + b * b;
}



SpaceModel::Vector SpaceModel::createVector(double x, double y, double z) {
	return create(x, y, z);
}

SpaceModel::Basis SpaceModel::createBasis(Vector const &i, Vector const &j, Vector const &k) {
	return create(i, j, k);
}

template <class T>
std::vector<T>  SpaceModel::create(T const &item1, T const &item2, T const &item3) {
	std::vector<T> result(3);
	result[0] = item1;
	result[1] = item2;
	result[2] = item3;
	return result;
}
