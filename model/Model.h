#ifndef MODEL_H
#define MODEl_H

#include <QObject>

#include "utils/CommonDefs.h"
#include "core/Approximation.h"

class Model : public QObject 
{
	Q_OBJECT
	private:
	
	class Function {
		public:
		std::string name;
		double (*f)(double, double);
		Function(std::string, double (*)(double, double));
	};
	
	std::size_t nx, ny;
	std::size_t mx, my;

	double ax, bx;
	double ay, by;

	std::vector<Function> functions;

	Mesh *mesh;
	Data *data;

	Approx::Approximator approximator;
	
	public:
	Model(double, double, double, double, std::size_t, std::size_t, std::size_t, std::size_t, int, Mesh *, Data *, QObject *);
	
	void errorUp();
	void errorDown();
	
	void changeFunc();
	void changeMode();
	void downTriangulation();
	void upTriangulation();

	void numberDown();
	void numberUp();
	
	private:
	void updateBound();

	void remake(std::vector<R3Geometry::Point> &, double (Approx::Approximator::*) (double, double));
	
	void makeOrigin(std::vector<R3Geometry::Point> &);
	void makeApprox(std::vector<R3Geometry::Point> &);
	void makeResudial(std::vector<R3Geometry::Point> &);
	
	void update();
	
	signals:
	void updateMesh();
	void updateData();
};

#endif // MODEl_H
