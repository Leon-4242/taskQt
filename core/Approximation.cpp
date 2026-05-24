#include "Approximation.h"
#include <sys/time.h>
#include <iostream>

namespace Approx {

	Approximator::Approximator(double ax, double bx, double ay, double by, int nx, int ny, double (*f)(double, double) ): 
		ax(ax), bx(bx), 
		ay(ay), by(by),
		nx(nx), ny(ny), 
		p(0), f(f), 
		pixel(0.01), mid(R2Geometry::Point(ax+nx*(bx-ax)/(2*(nx-1)), ay+ny*(by-ay)/(2*(ny-1)))),
		values(std::vector<double>((2*nx-1)*(2*ny-1))), coeff(std::vector<double>(6*2*(nx-1)*(ny-1)))
	{
		double dx = (bx-ax)/(2*(nx-1)), dy = (by-ay)/(2*(ny-1));

		for (int i = 0; i < 2*nx-1; ++i) {
			for (int j = 0; j < 2*ny-1; ++j) {
				values[i*(2*ny-1)+j] = f(ax+i*dx, ay+j*dy) + (i == nx && j == ny? p : 0);
			}
		}

		makeApproxPiecePolynom();
	}

	void Approximator::makeApproxPiecePolynom(void)
	{
		
	#if TIMER==1	
		struct timeval start, end;
		long long start_us, end_us;

		gettimeofday(&start, NULL);
	#endif

		double fP1, fP2, fP3, fP4, fP5, fP6;
		for (int i = 0; i < nx-1; ++i) {
			for (int j = 0; j < ny-1; ++j) {
				fP1 = values[2*i*(2*ny-1)+2*j+2],
				fP2 = values[2*i*(2*ny-1)+2*j], 
				fP3 = values[(2*i+2)*(2*ny-1)+2*j], 
				fP4 = values[2*i*(2*ny-1)+2*j+1], 
				fP5 = values[(2*i+1)*(2*ny-1)+2*j], 
				fP6 = values[(2*i+1)*(2*ny-1)+2*j+1];

				coeff[2*6*(i*(ny-1)+j) + 0] = 2*fP2+2*fP3-4*fP5;
				coeff[2*6*(i*(ny-1)+j) + 1] = 4*fP2-4*fP4-4*fP5+4*fP6;
				coeff[2*6*(i*(ny-1)+j) + 2] = 2*fP1+2*fP2-4*fP4;
				coeff[2*6*(i*(ny-1)+j) + 3] = -3*fP2-fP3+4*fP5;
				coeff[2*6*(i*(ny-1)+j) + 4] = -fP1-3*fP2+4*fP4;
				coeff[2*6*(i*(ny-1)+j) + 5] = fP2;

				fP1 = values[2*i*(2*ny-1)+2*j+2],
				fP2 = values[(2*i+2)*(2*ny-1)+2*j+2], 
				fP3 = values[(2*i+2)*(2*ny-1)+2*j], 
				fP4 = values[(2*i+1)*(2*ny-1)+2*j+2], 
				fP5 = values[(2*i+2)*(2*ny-1)+2*j+1], 
				fP6 = values[(2*i+1)*(2*ny-1)+2*j+1];

				coeff[2*6*(i*(ny-1)+j) + 6] = 2*fP1+2*fP2-4*fP4;
				coeff[2*6*(i*(ny-1)+j) + 7] = 4*fP2-4*fP4-4*fP5+4*fP6;
				coeff[2*6*(i*(ny-1)+j) + 8] = 2*fP2+2*fP3-4*fP5;
				coeff[2*6*(i*(ny-1)+j) + 9] = -3*fP1-5*fP2+8*fP4+4*fP5-4*fP6;
				coeff[2*6*(i*(ny-1)+j) + 10] = -5*fP2-3*fP3+4*fP4+8*fP5-4*fP6;
				coeff[2*6*(i*(ny-1)+j) + 11] = fP1+3*fP2+fP3-4*fP4-4*fP5+4*fP6;
			}
		}

	#if TIMER==1
		gettimeofday(&end, NULL);
 
		start_us = start.tv_sec * 1000000 + start.tv_usec;
		end_us = end.tv_sec * 1000000 + end.tv_usec;
 
		std::cerr <<"\n\n\n\n\n\n\n\n\n\n" << (double)(end_us - start_us) / 1000000. << "\n\n\n\n\n\n\n\n\n\n"<< std::endl;
	#endif

		return;
	}	
	
	double Approximator::origin(double x, double y) 
	{
		return f(x, y) + (fabs(x-mid.X()) < pixel && fabs(y-mid.Y()) < pixel ? p : 0);
	}

	double Approximator::approxPiecePolynom(double x, double y)
	{
		double invStepX = (nx-1)/(bx-ax), invStepY = (ny-1)/(by-ay);
		int i = floor((x-ax)*invStepX), j = floor((y-ay)*invStepY);
		
		if (i < 0)
			i = 0;
		if (i >= nx-1)
			i = nx-2;

		if (j < 0)
			j = 0;
		if (j >= ny-1)
			j = ny-2;

		double xi = (x-ax)*invStepX-i, eta = (y-ay)*invStepY-j;
		if (xi+eta <= 1)
			return xi*(coeff[2*6*(i*(ny-1)+j)]*xi+coeff[2*6*(i*(ny-1)+j)+1]*eta+coeff[2*6*(i*(ny-1)+j)+3]) + 
				eta*(eta*coeff[2*6*(i*(ny-1)+j)+2]+coeff[2*6*(i*(ny-1)+j)+4]) + coeff[2*6*(i*(ny-1)+j)+5];
		else
			return xi*(coeff[2*6*(i*(ny-1)+j)+6]*xi+coeff[2*6*(i*(ny-1)+j)+7]*eta+coeff[2*6*(i*(ny-1)+j)+9]) + 
				eta*(eta*coeff[2*6*(i*(ny-1)+j)+8]+coeff[2*6*(i*(ny-1)+j)+10]) + coeff[2*6*(i*(ny-1)+j)+11];
	}

	double Approximator::errorPiecePolynom(double x, double y)
	{
		return fabs(f(x, y)-approxPiecePolynom(x, y));
	}

	void Approximator::setFunction(double (*func)(double, double))
	{
		f = func;
		p = 0;

		double dx = (bx-ax)/(2*(nx-1)), dy = (by-ay)/(2*(ny-1));

		for (int i = 0; i < 2*nx-1; ++i) {
			for (int j = 0; j < 2*ny-1; ++j) {
				values[i*(2*ny-1)+j] = f(ax+i*dx, ay+j*dy) + (i == nx && j == ny? p : 0);
			}
		}

		makeApproxPiecePolynom();
	}

	void Approximator::setError(double pp)
	{
		p = pp;
		values[nx * (2*ny-1) + ny] = f(ax+nx*(bx-ax)/(2*(nx-1)), ay+ny*(by-ay)/(2*(ny-1))) + p;

		makeApproxPiecePolynom();
	}

	
	void Approximator::setN(int nxx, int nyy)
	{
		nx = nxx;
		ny = nyy;
	
		values.resize((2*nx-1)*(2*ny-1));
		coeff.resize(6*2*nx*ny);

		double dx = (bx-ax)/(2*(nx-1)), dy = (by-ay)/(2*(ny-1));

		for (int i = 0; i < 2*nx-1; ++i) {
			for (int j = 0; j < 2*ny-1; ++j) {
				values[i*(2*ny-1)+j] = f(ax+i*dx, ay+j*dy) + (i == nx && j == ny? p : 0);
			}
		}

		mid = R2Geometry::Point(ax+nx*dx, ay+ny*dy);

		makeApproxPiecePolynom();
	}

	void Approximator::setPixel(double Pixel) 
	{
		pixel = Pixel;
	}

	std::pair<double, double> Approximator::minMaxChangeable(double (Approximator::*ff)(double, double))
	{	
		std::pair<double, double> minMax;
		minMax.first = (this->*ff)(ax, ay);
		minMax.second = (this->*ff)(ax, ay);
		double xDelta = 0.01;
		double yDelta = 0.01;
		for (double x = ax; x - bx < EPSILON_FOR_COMPARE; x += xDelta)
		{
			for (double y = ay; y - by < EPSILON_FOR_COMPARE; y += yDelta) {
				double z = (this->*ff)(x, y);
				if (z < minMax.first)
					minMax.first = z;
				if (z > minMax.second)
					minMax.second = z;
			}
		}
		return minMax;
	}

	std::vector<std::pair<double, double>> Approximator::minMax(void)
	{	
		std::vector<std::pair<double, double>> minMax{};
		minMax.push_back(minMaxChangeable(&Approximator::origin));
		minMax.push_back(minMaxChangeable(&Approximator::approxPiecePolynom));
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
