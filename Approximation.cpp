#include "Approximation.h"
#include <iostream>

namespace Approx {

	Approximator::Approximator(double a, double b, int n, double (*f)(double) ): 
		a(a), b(b), n(n), p(0), f(f), pixel(0),
		chebushevPoints(std::vector<double>(n)), diff(std::vector<double>(n)), 
		points(std::vector<double>(n)), values(std::vector<double>(n)),
		xi(std::vector<double>(n-3)), v(std::vector<double>(n-3)), 
		dUp(std::vector<double>(n-4)), d(std::vector<double>(n-3)), dDown(std::vector<double>(n-4)),
		coeff(std::vector<double>(3*(n-2)))
	{
		if (n < 50) {
			for (int i = 0; i < n; ++i) {
				chebushevPoints[i] = (a+b)/2 + ((b-a)/2) * cos(M_PI*(i+0.5)/n);
				diff[i] = f(chebushevPoints[i]) + (i == n/2 ? p : 0);
			}

			makeApproxPolynom();
		}

		for (int i = 0; i < n; ++i) {
			points[i] = a + (b-a)/(n-1) *i; 
			values[i] = f(points[i]) + (i == n/2 ? p : 0);
		}

		makeApproxPiecePolynom();
	}

	void Approximator::makeApproxPolynom(void)
	{
		for (int t = 1; t < n; ++t) {
			for (int k = n-1; k >= t; --k) {
				diff[k] = (diff[k]-diff[k-1])/(chebushevPoints[k]-chebushevPoints[k-t]);
			}
		}
	}

	void Approximator::makeApproxPiecePolynom(void)
	{
		for (int i = 0; i < n-3; ++i)
			xi[i] = (points[i-1 + 2]+points[i + 2])/2;

		d[0] = 1/(xi[0]-points[0]) + 1/(xi[0]-points[1]) + 1/(points[2]-xi[0]) + 1/(xi[1]-xi[0]);
		dUp[0] = 1/(xi[1]-points[2]) - 1/(xi[1]-xi[0]);
		v[0] = 
			values[1]*(1/(xi[0]-points[0]) + 1/(xi[0]-points[1])) + 
			values[2]*(1/(points[2]-xi[0]) + 1/(xi[1]-points[2])) +
			((xi[0]-points[1])/(xi[0]-points[0]))*(values[1]-values[0])/(points[1]-points[0]);

		double div1, div2, div3, div4, div5, div6;
		for (int i = 1; i < n-4; ++i) {
			div1 = 1/(points[(i-1)+2]-xi[i-1]);
			div2 = 1/(xi[i]-points[(i-1)+2]);
			div3 = 1/(points[i+2]-xi[i]);
			div4 = 1/(xi[i+1]-points[i+2]);
			div5 = 1/(xi[i]-xi[i-1]);
			div6 = 1/(xi[i+1]-xi[i]);

			dDown[i-1] = div1 - div5;
			d[i] = div2 + div5 + div3 + div6;
			dUp[i] = div4 - div6;
			v[i] = values[(i-1)+2]*(div1 + div2) + values[i+2]*(div3 + div4);
		}

		dDown[n-5] = 1/(points[n-3]-xi[n-5]) - 1/(xi[n-4]-xi[n-5]);
		d[n-4] = 1/(xi[n-4]-points[n-3]) + 1/(xi[n-4]-xi[n-5])+ 1/(points[n-2]-xi[n-4]) + 1/(points[n-1]-xi[n-4]);
		v[n-4] = 
			values[n-3]*(1/(points[n-3]-xi[n-5]) + 1/(xi[n-4]-points[n-3])) + 
			values[n-2]*(1/(points[n-2]-xi[n-4]) + 1/(points[n-1]-xi[n-4])) -
			((points[n-2]-xi[n-4])/(points[n-1]-xi[n-4]))*(values[n-1]-values[n-2])/(points[n-1]-points[n-2]);

		solve(dUp, d, dDown, v, n-3);
	
		coeff[0] = values[0];
		double diff = (values[1]-values[0])/(points[1]-points[0]);
		coeff[1] = diff - ((points[1]-points[0])/(xi[0]-points[0]))*((v[0]-values[1])/(xi[0]-points[1])-diff);
		coeff[2] = 1/(xi[0]-points[0]) *((v[0]-values[1])/(xi[0]-points[1])-diff);

		for (int i = 1; i < n-3; ++i) {
			coeff[i*3] = v[i-1];
			
			div1 = 1/(points[i+1]-xi[i-1]);
			div2 = 1/(xi[i]-xi[i-1]);
			div3 = 1/(xi[i]-points[i+1]);

			coeff[i*3+1] = (values[i+1]-v[i-1])*div1-(points[i+1]-xi[i-1])*div2*
					((v[i]-values[i+1])*div3 - (values[i+1]-v[i-1])*div1);
			coeff[i*3+2] = div2*((v[i]-values[i+1])*div3 - (values[i+1]-v[i-1])*div1);
		}
		
		coeff[(n-3)*3] = v[n-4];
		coeff[(n-3)*3+1] = (values[n-2]-v[n-4])/(points[n-2]-xi[n-4]) - (points[n-2]-xi[n-4])/(points[n-1]-xi[n-4])*
			((values[n-1]-values[n-2])/(points[n-1]-points[n-2])-(values[n-2]-v[n-4])/(points[n-2]-xi[n-4]));
		coeff[(n-3)*3+2] = 1/(points[n-1]-xi[n-4])*((values[n-1]-values[n-2])/(points[n-1]-points[n-2])-(values[n-2]-v[n-4])/(points[n-2]-xi[n-4]));

		return;
	}

	double Approximator::approxPolynom(double x)
	{
		if (n > 50)
			return 0;
		double res = diff[n-1];
		for (int i = n-2; i >= 0; --i) {
			res = res*(x-chebushevPoints[i]) + diff[i];
		}	
		return res;
	}

	double Approximator::approxPiecePolynom(double x)
	{
		if (x < xi[0])
			return (coeff[2]*(x-points[0])+coeff[1])*(x-points[0])+coeff[0];

		if (x > xi[n-4])
			return (coeff[3*(n-3)+2]*(x-xi[n-4])+coeff[3*(n-3)+1])*(x-xi[n-4])+coeff[3*(n-3)];

		int l = 0;
		int r = n-3;
		int k =(l+r)/2;
		while (l <= r) {
			if (xi[k] < x && x <= xi[k+1])
				break;
			if (x <= xi[k])
				r = k-1;
			else if (x > xi[k+1])
				l = k+1;

			k = (l+r)/2;
		}

		return (coeff[3*(k+1)+2]*(x-xi[k])+coeff[3*(k+1)+1])*(x-xi[k])+coeff[3*(k+1)];
	}

	double Approximator::approxPiecePolynomOptimized(double x, int &k) 
	{
		if (x < xi[0])
			return (coeff[2]*(x-points[0])+coeff[1])*(x-points[0])+coeff[0];

		if (x > xi[n-4])
			return (coeff[3*(n-3)+2]*(x-xi[n-4])+coeff[3*(n-3)+1])*(x-xi[n-4])+coeff[3*(n-3)];

		while (k < n-3 && x > xi[k+1])
			++k;

		return (coeff[3*(k+1)+2]*(x-xi[k])+coeff[3*(k+1)+1])*(x-xi[k])+coeff[3*(k+1)];
	}


	double Approximator::errorPolynom(double x)
	{
		if ( n > 50)
			return 0;

		return fabs(f(x)-approxPolynom(x));
	}

	double Approximator::errorPiecePolynom(double x)
	{
		return fabs(f(x)-approxPiecePolynom(x));
	}

	void Approximator::setFunction(double (*func)(double))
	{
		f = func;
		p = 0;
		
		if (n < 50) {
			for (int i = 0; i < n; ++i)
				diff[i] = f(chebushevPoints[i]);

			makeApproxPolynom();
		}

		for (int i = 0; i < n; ++i)
			values[i] = f(points[i]);

		makeApproxPiecePolynom();
	}

	void Approximator::setError(double pp)
	{
		p = pp;

		if (n < 50) {
			diff[n/2] = f(chebushevPoints[n/2]) + p;

			makeApproxPolynom();
		}

		values[n/2] = f(points[n/2]) + p;

		makeApproxPiecePolynom();
	}

	
	void Approximator::setN(int nn)
	{
		n = nn;
	
		if (n < 50) {
			chebushevPoints.resize(n);
			diff.resize(n);

			for (int i = 0; i < n; ++i) {
				chebushevPoints[i] = (a+b)/2 + ((b-a)/2) * cos(M_PI*(i+0.5)/n);
				diff[i] = f(chebushevPoints[i]) + (i == n/2 ? p : 0);
			}

			makeApproxPolynom();
		}

		points.resize(n);
		values.resize(n);
	
		xi.resize(n-3);
		v.resize(n-3);

		dUp.resize(n-4);
		d.resize(n-3);
		dDown.resize(n-4);

		coeff.resize(3*(n-2));

		for (int i = 0; i < n; ++i) {
			points[i] = a + (b-a)/(n-1) *i; 
			values[i] = f(points[i]) + (i == n/2 ? p : 0);
		}

		makeApproxPiecePolynom();
	}

	void Approximator::setPixel(double Pixel) 
	{
		pixel = Pixel;
	}

	double Approximator::origin(double x) 
	{
		return f(x) + (fabs(x-(a + n/2 * (b-a)/(n-1)) ) < 2*pixel ? p : 0);
	}
	
	std::pair<double, double> Approximator::minMaxChangeable(double (Approximator::*ff)(double))
	{	
		std::pair<double, double> minMax;
		minMax.first = (this->*ff)(a);
		minMax.second = (this->*ff)(a);
		double xDelta = 0.01;
		for (double x = a; x - b < EPSILON_FOR_COMPARE; x += xDelta)
		{
			double y = (this->*ff)(x);
			if (y < minMax.first)
				minMax.first = y;
			if (y > minMax.second)
				minMax.second = y;
		}
		return minMax;
	}
	std::vector<std::pair<double, double>> Approximator::minMax(void)
	{	
		std::vector<std::pair<double, double>> minMax{};
		minMax.push_back(minMaxChangeable(&Approximator::origin));
		minMax.push_back(minMaxChangeable(&Approximator::approxPolynom));
		minMax.push_back(minMaxChangeable(&Approximator::approxPiecePolynom));
		minMax.push_back(minMaxChangeable(&Approximator::errorPolynom));
		minMax.push_back(minMaxChangeable(&Approximator::errorPiecePolynom));
		return minMax;
	}

	void solve(std::vector<double> &dUp, std::vector<double> &d, std::vector<double> &dDown, std::vector<double> &b, int n)
	{
		
		double div;
		for (int i = 0; i < n-1; ++i) {
			div = 1/d[i];
			b[i] *= div;
			dUp[i] *= div;

			b[i+1] -= b[i]*dDown[i];
			d[i+1] -= dUp[i]*dDown[i];
		}

		b[n-1] /= d[n-1];

		for (int i = n-1; i > 0; --i)
			b[i-1] -= b[i]*dUp[i-1];
	}
}
