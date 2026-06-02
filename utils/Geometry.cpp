#include "Geometry.h"

namespace Geometry {

	Quaternion::Quaternion(double w, double x, double y, double z):
		Vector<4>(w, x, y, z)
	{
		double mod = sqrt(coord[0]*coord[0]+coord[1]*coord[1]+coord[2]*coord[2]+coord[3]*coord[3]);
		if (mod < EPS) {
			coord[0] = coord[1] = coord[2] = coord[3] = 0.0;
			m = {
				1.0, 0.0, 0.0, 
				0.0, 1.0, 0.0, 
				0.0, 0.0, 1.0
			};	
		} else {
			coord[0] /= mod;
			coord[1] /= mod;
			coord[2] /= mod;
			coord[3] /= mod;

			const double xx = coord[1]*coord[1];
			const double yy = coord[2]*coord[2];
			const double zz = coord[3]*coord[3];
	
			const double xy = coord[1]*coord[2];
			const double xz = coord[1]*coord[3];
			const double yz = coord[2]*coord[3];

			const double wx = coord[0]*coord[1];
			const double wy = coord[0]*coord[2];
			const double wz = coord[0]*coord[3];
	
			m = {
				1.0 - 2.0*(yy + zz), 2.0*(xy - wz), 2.0*(xz + wy), 
				2.0*(xy + wz), 1.0 - 2.0*(xx + zz), 2.0*(yz - wx), 
				2.0*(xz - wy), 2.0*(yz + wx), 1.0 - 2.0*(xx + yy)
			};	
		}
	}

	Quaternion::Quaternion(const Vector<3> &axis, double angle)
	{
		if (axis.mod() < EPS) {
			coord[0] = 1.0;
			coord[1] = 0;
			coord[2] = 0;
			coord[3] = 0;

			m = {
				1.0, 0.0, 0.0, 
				0.0, 1.0, 0.0, 
				0.0, 0.0, 1.0
			};	
		} else {
			Vector<3> n = axis.unit();
			
			double half = angle * 0.5;
			double s = sin(half);

			coord[0] = cos(half);
			coord[1] = n.X() * s;
			coord[2] = n.Y() * s;
			coord[3] = n.Z() * s;

			const double xx = coord[1]*coord[1];
			const double yy = coord[2]*coord[2];
			const double zz = coord[3]*coord[3];

			const double xy = coord[1]*coord[2];
			const double xz = coord[1]*coord[3];
			const double yz = coord[2]*coord[3];

			const double wx = coord[0]*coord[1];
			const double wy = coord[0]*coord[2];
			const double wz = coord[0]*coord[3];

			m = {
				1.0 - 2.0*(yy + zz), 2.0*(xy - wz), 2.0*(xz + wy), 
				2.0*(xy + wz), 1.0 - 2.0*(xx + zz), 2.0*(yz - wx), 
				2.0*(xz - wy), 2.0*(yz + wx), 1.0 - 2.0*(xx + yy)
			};	
		}
	}

	Quaternion Quaternion::inverse() const
	{
		return Quaternion(coord[0], -coord[1], -coord[2], -coord[3]);
	}

	Quaternion Quaternion::operator*(const Quaternion &rhs) const
	{
		return Quaternion(
				coord[0]*rhs.coord[0] - (coord[1]*rhs.coord[1]+coord[2]*rhs.coord[2]+coord[3]*rhs.coord[3]), 
				coord[0]*rhs.coord[1] +rhs.coord[0]*coord[1] + (coord[2]*rhs.coord[3] - coord[3]*rhs.coord[2]),
				coord[0]*rhs.coord[2] +rhs.coord[0]*coord[2] + (coord[3]*rhs.coord[1] - coord[1]*rhs.coord[3]),
				coord[0]*rhs.coord[3] +rhs.coord[0]*coord[3] + (coord[1]*rhs.coord[2] - coord[2]*rhs.coord[1])
				);
	}

	Vector<3> Quaternion::rotate(const Vector<3> &v) const
	{
		return m*v;
	}
}
