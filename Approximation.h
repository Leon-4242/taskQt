#ifndef APPROXIMATION_H
#define APPROXIMATION_H

#include "R2Geometry.h"
#include "CommonDefs.h"
#include <cassert>
#include <vector>
#include <functional>

namespace Approx {
	using namespace R2Geometry;

	class Approximator {
		double a, b;
		int n;
		double p;

		double (*f)(double);

		double pixel;

		std::vector<double> chebushevPoints;
		std::vector<double> diff;

		std::vector<double> points;
		std::vector<double> values;

		std::vector<double> xi;
		std::vector<double> v;

		/*
		std::vector<double> dUp;
		std::vector<double> d;
		std::vector<double> dDown;
		std::vector<double> right;
		*/
		std::vector<double> system;

		std::vector<double> coeff;

		void makeApproxPolynom(void);
		void makeApproxPiecePolynom(void);

		std::pair<double, double> minMaxChangeable(double (Approximator::*)(double));

		public:
		Approximator(double, double, int, double (*)(double) = [](double x) {return x;});

		double approxPolynom(double);
		double approxPiecePolynom(double);
		double approxPiecePolynomOptimized(double, int &);
		double errorPolynom(double);
		double errorPiecePolynom(double);

		double origin(double);

		std::vector<std::pair<double, double>> minMax(void);

		void setFunction(double (*)(double));
		void setError(double);
		void setN(int);
		void setPixel(double);
	};

	void solve(std::vector<double> &, /*std::vector<double> &, std::vector<double> &, std::vector<double> &, */int, std::vector<double> &);	
}

#endif //APPROXIMATION_H
