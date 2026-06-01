#ifndef COMMONDEFS_H
#define COMMONDEFS_H

#define EPSILON_FOR_COMPARE (1e-15)

#include "utils/Geometry.h"
#include <QPointF>
#include <QColor>

namespace R2Geometry {
	using Vector = Geometry::Vector<2>;
	using Point = Geometry::Point<2>;
}

namespace R3Geometry {
	using Vector = Geometry::Vector<3>;
	using Point = Geometry::Point<3>;
}


class Mesh
{
	public:
	
	class Triangle
	{
		public:
		int a, b, c;
		double depth;
		
		Triangle (int a = 0, int b = 0, int c = 0, double depth = 0);
		Triangle (const Triangle &tr);
		Triangle (Triangle && tr);
		Triangle & operator= (const Triangle &tr);
		Triangle & operator= (Triangle && tr);
	};	
	
	std::vector<R3Geometry::Point> points;
	std::vector<Triangle> triangles; 
		
	Mesh(void)
	{}

	Mesh& operator= (const Mesh &mesh)
	{
		points = mesh.points;
		triangles = mesh.triangles;
		return *this;
	}

	void swap (Mesh &mesh)
	{
		points.swap(mesh.points);
		triangles.swap(mesh.triangles);
	}
};

#endif // COMMONDEFS_H
