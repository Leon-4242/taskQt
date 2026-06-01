#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <QObject> 
#include <QPointF>
#include <QColor>

#include "utils/CommonDefs.h"

class Storage : public QObject
{
	Q_OBJECT
	public:

	using Polygon = struct 
	{
	    QPointF vertex[4];
		int n;
		double depth;
		QColor color;
	    bool visible;
	};

	class Triangle
	{
		public:
		int a, b, c;
		double depth;
		
		Triangle (int a = 0, int b = 0, int c = 0, double depth = 0);
		Triangle (const Storage::Triangle &tr);
		Triangle (Storage::Triangle && tr);
		Triangle & operator= (const Triangle &tr);
		Triangle & operator= (Triangle && tr);

	};

	class Mesh
	{
		public:
		
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

	private:

	bool updateable;
	Mesh currentFrame;
	Mesh readyFrame;
	Mesh newFrame;

	public:

	Storage(QObject *object = nullptr);

	Mesh * renderrAccess();
	Mesh * getAccess();

	public slots:

	void updatedMesh();
	void updateRenderr();

	signals:

	void update();
};

#endif //STORAGE_H
