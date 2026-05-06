#include "R2Geometry.h"

namespace R2Geometry {
	Vector operator+ (const Vector& u, const Vector& v) {
		return Vector(u.X()+v.X(), u.Y()+v.Y());
	}

	Vector operator- (const Vector& u, const Vector& v) {
		return Vector(u.X()-v.X(), u.Y()-v.Y());
	}

	Point operator+ (const Vector& v, const Point& p) {
		return Point(v.X()+p.X(), v.Y()+p.Y());
	}

	Vector operator+(const Vector& v) {
		return Vector(v.X(), v.Y());
	}

	Vector operator-(const Vector& v) {
		return Vector(-v.X(), -v.Y());
	}

	double operator*(const Vector& u, const Vector v) {
		return u.X()*v.X()+u.Y()*v.Y();
	}

	Vector operator*(const Vector& v, const double c) {
		return Vector(c*v.X(), c*v.Y());
	}

	Vector operator*(const double c, const Vector& v) {
		return Vector(c*v.X(), c*v.Y());
	}

	Point operator* (const double c, const Point& p) {
		return Point(c*p.X(), c*p.Y());
	}

	Point operator* (const Point& p, const double c) {
		return Point(c*p.X(), c*p.Y());
	}

	Vector RVector (const Point& p) {
		return Vector(p.X(), p.Y());
	}

	Point operator+ (const Point& p, const Vector& v) {
		return Point(p.X()+v.X(), p.Y()+v.Y());
	}

	Point operator+ (const Point& p, const Point& q) {
		return Point (q.X() + p.X(), q.Y() + p.Y());
	}

	Vector operator- (const Point& p, const Point& q) {
		return Vector(p.X()-q.X(), p.Y()-q.Y());
	}

	Point RPoint (const Vector& v) {
		return Point(v.X(), v.Y());
	}
	
}
