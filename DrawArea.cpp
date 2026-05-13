#include "DrawArea.h"

Function::Function (std::string name, double(*f)(double)): name(name), f(f) 
{}

DrawArea::DrawArea (QWidget *parent, double a, double b, int n, int k, Data *data) :
	QWidget(parent),
	a(a), b(b),
	bgColor(Qt::lightGray),
	xMin(a - 0.01*(b-a)),
	xMax(b + 0.01*(b-a)),
	yMin(0.),
	yMax(0.),
	xCoeff(50.),		
	yCoeff(50.),		
	data(data),
	pointsDrawFlag(true),
	approximator(a, b, n)
{
	setAttribute(Qt::WA_StaticContents); // for optimizing painting events

	functions = std::vector<Function>
	{
		Function("f(x) = 1", [](double x) {(void)x; return 1.0;}),
		Function("f(x) = x", [](double x) {return x;}),
		Function("f(x) = x^2", [](double x) {return x*x;}),
		Function("f(x) = x^3", [](double x) {return x*x*x;}),
		Function("f(x) = x^4", [](double x) {return x*x*x*x;}),
		Function("f(x) = e^x", [](double x) {return exp(x);}),
		Function("f(x) = 1/(25x^2+1)", [](double x) {return 1/(25*x*x+1);}),
	};
	
	data->k = k;
	data->name = functions[k].name;

	data->origin = true;
	data->polynom = true;
	data->piecePolynom = false;
	data->error = false;
	data->s = 0;
	data->n = n;
	data->p = 0;

	approximator.setFunction(functions[data->k].f);
}

void DrawArea::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing, false);
	recomputeBounds(); 
	initializeMap();
	drawCoordSystem(&painter);

	approximator.setPixel(mapFromPixels(QPointF(1., 0.)).X() - mapFromPixels(QPointF(0., 0.)).X());

	painter.setRenderHint(QPainter::Antialiasing, true);
	
	if (data->origin)
		drawOrigin(&painter);
	if (data->polynom) {
		drawApproxFirst(&painter);
		if (pointsDrawFlag)
			drawPointsFirst(&painter);
	}
	if (data->piecePolynom) {
		drawApproxSecond(&painter);
		if (pointsDrawFlag)
			drawPointsSecond(&painter);
	}
	if (data->error)
		drawError(&painter);

	drawInformation(&painter);
	event->accept();
}

void DrawArea::drawInformation(QPainter * /*painter*/) 
{
	/*
	*/
}

void DrawArea::drawCoordSystem(QPainter *painter) 
{
	int w = width();
	int h = height();
	if (w == 0 || h == 0)
		return;

	painter->setBrush(QBrush(bgColor));
	painter->drawRect(0, 0, w, h);
	painter->setBrush(Qt::NoBrush);

	QPen penGray(Qt::gray);
	penGray.setWidth(0);
	painter->setPen(penGray);
	
	QPen penBlack(Qt::black);
	penBlack.setWidth(2);
	painter->setPen(penBlack);
	
	if (yMin < 0.0 && 0.0 < yMax) {
		painter->drawLine(mapToPixels(Approx::Point(xMin, 0)), mapToPixels(Approx::Point(xMax, 0)));
	}
	if (xMin < 0.0 && 0.0 < xMax) {
		painter->drawLine(mapToPixels(Approx::Point(0, yMin)), mapToPixels(Approx::Point(0, yMax)));
	}

	QPointF center = mapToPixels(Approx::Point(0, 0));
	
	QPointF dx(8., 0.);
	QPointF dy(0., 8.);

	QPointF xTick(100., 0);
	QPointF yTick(0, 100.);

	painter->drawText(center.x()+10, center.y()-10, QString("0"));
	int i = 0;
	while ((center+(++i)*xTick).x() < w) {
		painter->drawLine((center+i*xTick)-dy, (center+i*xTick)+dy);
		painter->drawText((center+i*xTick).x(), (center+i*xTick).y() - 20, QString::fromStdString(std::to_string(mapFromPixels(center+i*xTick).X())));
	}

	i = 0;
	while ((center+(--i)*xTick).x() > 0) {
		painter->drawLine((center+i*xTick)-dy, (center+i*xTick)+dy);
		painter->drawText((center+i*xTick).x(), (center+i*xTick).y() - 20, QString::fromStdString(std::to_string(mapFromPixels(center+i*xTick).X())));
	}

	i = 0;
	while ((center+(++i)*yTick).y() < h) {
		painter->drawLine((center+i*yTick)-dx, (center+i*yTick)+dx);
		painter->drawText((center+i*yTick).x() + 20, (center+i*yTick).y(), QString::fromStdString(std::to_string(mapFromPixels(center+i*yTick).Y())));
	}

	i = 0;
	while ((center+(--i)*yTick).y() > 0) {
		painter->drawLine((center+i*yTick)-dx, (center+i*yTick)+dx);
		painter->drawText((center+i*yTick).x() + 20, (center+i*yTick).y(), QString::fromStdString(std::to_string(mapFromPixels(center+i*yTick).Y())));
	}

}

void DrawArea::drawPointsFirst(QPainter *painter) 
{
	QPen bluePen(Qt::darkGreen);
	bluePen.setWidth(2);

	painter->setPen(bluePen);

	QPointF dx(8., 0.);
	QPointF dy(0., 8.);
	
	int counter  = 0;
	double x = std::max(xMin, (a+b)/2 + ((b-a)/2) * cos(M_PI*0.5/data->n));
	double y = functions[data->k].f(x) + (counter == data->n/2  ? data->p*0.1*data->originAbsMax : 0);
	
	while (counter < data->n) {
		QPointF t = mapToPixels(Approx::Point(x, y));
		painter->drawLine(t-dx, t+dx);
		painter->drawLine(t-dy, t+dy);

		++counter;
		x = (a+b)/2 + ((b-a)/2) * cos(M_PI*(counter+0.5)/data->n);
		y = functions[data->k].f(x) + (counter == data->n/2  ? data->p*0.1*data->originAbsMax : 0);
	}
}

void DrawArea::drawPointsSecond(QPainter *painter) 
{
	QPen bluePen(Qt::blue);
	bluePen.setWidth(2);

	painter->setPen(bluePen);

	QPointF dx(8., 0.);
	QPointF dy(0., 8.);
	
	int counter  = 0;
	double x = std::max(xMin, a);
	double y = functions[data->k].f(x) + (counter == data->n/2  ? data->p*0.1*data->originAbsMax : 0);
	
	while (x - std::min(xMax, b) < EPSILON_FOR_COMPARE) {
		QPointF t = mapToPixels(Approx::Point(x, y));
		painter->drawLine(t-dx, t+dx);
		painter->drawLine(t-dy, t+dy);

		++counter;
		x += (b-a)/(data->n-1);
		y = functions[data->k].f(x) + (counter == data->n/2  ? data->p*0.1*data->originAbsMax : 0);
	}
}


void DrawArea::drawFunction(QPainter *painter, double (Approx::Approximator::*function)(double)) 
{
	QPainterPath path;
	
	Approx::Point h0 = mapFromPixels(QPointF(0., 0.));
	Approx::Point h1 = mapFromPixels(QPointF(1., 0.));
    
	double xDelta = h1.X() - h0.X();

	double x = std::max(xMin, a);
	double y = (approximator.*function)(x);

	path.moveTo(mapToPixels(Approx::Point(x, y)));

	while (x-std::min(xMax, b) < EPSILON_FOR_COMPARE) {
		x += xDelta;
		y = (approximator.*function)(x);
		path.lineTo(mapToPixels(Approx::Point(x, y)));
	}

	painter->drawPath(path);
}

void DrawArea::drawPolynomOptimized(QPainter *painter) 
{
	QPainterPath path;
	
	Approx::Point h0 = mapFromPixels(QPointF(0., 0.));
	Approx::Point h1 = mapFromPixels(QPointF(1., 0.));
    
	double xDelta = h1.X() - h0.X();

	int k = 0;

	double x = std::max(xMin, a);
	double y = approximator.approxPiecePolynomOptimized(x, k);

	path.moveTo(mapToPixels(Approx::Point(x, y)));

	while (x-std::min(xMax, b) < EPSILON_FOR_COMPARE) {
		x += xDelta;
		y = approximator.approxPiecePolynomOptimized(x, k);
		path.lineTo(mapToPixels(Approx::Point(x, y)));
	}

	painter->drawPath(path);
} 


void DrawArea::drawOrigin(QPainter *painter) 
{
	QPen pen(Qt::red);
	pen.setWidth(1);
	painter->setPen(pen);
	
	drawFunction(painter, &Approx::Approximator::origin);
} 


void DrawArea::drawApproxFirst(QPainter *painter) 
{
	if (data->n > 50)
		return;

	QPen pen(Qt::darkGreen);
	pen.setWidth(1);
	painter->setPen(pen);

	drawFunction(painter, &Approx::Approximator::approxPolynom);
}

void DrawArea::drawApproxSecond(QPainter *painter) 
{
	QPen pen(Qt::darkBlue);
	pen.setWidth(1);
	painter->setPen(pen);

	//drawPolynomOptimized(painter);
	drawFunction(painter, &Approx::Approximator::approxPiecePolynom);
}

void DrawArea::drawError(QPainter *painter) 
{
	if (data->n <= 50) {
		QPen penFirst(Qt::darkBlue);
		penFirst.setWidth(1);
		painter->setPen(penFirst);

		drawFunction(painter, &Approx::Approximator::errorPolynom);
	}

	QPen penSecond(Qt::darkRed);
	penSecond.setWidth(1);
	painter->setPen(penSecond);
	
	drawFunction(painter, &Approx::Approximator::errorPiecePolynom);
}

void DrawArea::resizeEvent(QResizeEvent *event) 
{
	if (width() == 0 || height() == 0)
		return;
		
	initializeMap();
	update();	
	event->accept();
}

void DrawArea::initializeMap() 
{
	int w = width();
	int h = height();
	if (w == 0 || h == 0)
		return;
	
	double dx = xMax - xMin;
	double dy = yMax - yMin;

	if (fabs(dx) < EPSILON_FOR_COMPARE || fabs(dy) < EPSILON_FOR_COMPARE)
		return;	
	
	xCoeff = double(w) / dx;
	yCoeff = double(h) / dy;
}

void DrawArea::recomputeBounds()
{
	std::vector<std::pair<double, double>> minMax = approximator.minMax();

	data->originAbsMax = std::max(fabs(minMax[0].first), fabs(minMax[0].second));
	data->polynomAbsMax = std::max(fabs(minMax[1].first), fabs(minMax[1].second));
	data->piecePolynomAbsMax = std::max(fabs(minMax[2].first), fabs(minMax[2].second));
	data->errorPolynomAbsMax = std::max(fabs(minMax[3].first), fabs(minMax[3].second));
	data->errorPiecePolynomAbsMax = std::max(fabs(minMax[4].first), fabs(minMax[4].second));

	double max, min;
	if (!data->origin) {
		max = std::max(minMax[3].second, minMax[4].second);
		min = std::max(minMax[3].first, minMax[4].first);
	} else if (!data->polynom) {
		max = std::max(minMax[0].second, minMax[2].second);
		min = std::min(minMax[0].first, minMax[2].first);
	} else if (!data->piecePolynom) {
		max = std::max(minMax[0].second, minMax[1].second);
		min = std::min(minMax[0].first, minMax[1].first);
	} else {
		max = std::max({minMax[0].second, minMax[1].second, minMax[2].second});
		min = std::min({minMax[0].first, minMax[1].first, minMax[2].first});
	}

	double yDelta = (max-min < 0.001 ? 1.5 : 0.01*(max-min));
	yMin = min - yDelta;
	yMax = max + yDelta;

	emit data->changed();
}



void DrawArea::changeFunc()
{
	data->k = (data->k+1)%functions.size();	
	data->name = functions[data->k].name;
	data->p = 0;

	approximator.setFunction(functions[data->k].f);

	emit data->changed();
	update();
}

void DrawArea::changeType()
{
	if (!data->origin) {
		data->error = false;
		data->origin = data->polynom = true;
	} else if (!data->polynom) {
		data->polynom = true;
	} else if (!data->piecePolynom) {
		data->piecePolynom = true;
		data->polynom = false;
	} else {
		data->origin = data->piecePolynom = data->polynom = false;
		data->error = true;
	}

	emit data->changed();

	update();
}

void DrawArea::scaleUp()
{
	if (xMax - xMin < 1e7) {
		double buff = (3.*xMin-xMax)/2.;
		xMax = (3.*xMax-xMin)/2.;
		xMin = buff;

		++(data->s);
	}

	approximator.setPixel(mapFromPixels(QPointF(1., 0.)).X() - mapFromPixels(QPointF(0., 0.)).X());

	emit data->changed();

	update();
}

void DrawArea::scaleDown()
{
	if (xMax - xMin > 1e-4) {
		double buff = (3.*xMin+xMax)/4.;
		xMax = (3.*xMax+xMin)/4.;
		xMin = buff;

		--(data->s);
	}

	approximator.setPixel(mapFromPixels(QPointF(1., 0.)).X() - mapFromPixels(QPointF(0., 0.)).X());

	emit data->changed();

	update();
}

void DrawArea::numberUp()
{
	if (data->n < 1e7) {
		data->n *= 2;
		approximator.setN(data->n);
	}

	emit data->changed();

	update();
}

void DrawArea::numberDown()
{
	if (data->n >= 10) {
		data->n /= 2;
		approximator.setN(data->n);
	}

	emit data->changed();

	update();
}

void DrawArea::errorUp()
{
	if (data->p < 10) {
		++(data->p);
		approximator.setError(data->p*0.1*data->originAbsMax);
	}

	emit data->changed();

	update();
}

void DrawArea::errorDown()
{
	if (data->p > -10) {
		--(data->p);
		approximator.setError(data->p*0.1*data->originAbsMax);
	}

	emit data->changed();

	update();
}

void DrawArea::points()
{
	pointsDrawFlag = !pointsDrawFlag;
	update();
}



QPointF DrawArea::mapToPixels(const Approx::Point &p) const 
{
	return QPointF((p.X() - xMin)*xCoeff, (yMax - p.Y())*yCoeff);
}

Approx::Point DrawArea::mapFromPixels(const QPointF &p) const 
{
	return Approx::Point(xMin + (p.x()/xCoeff), yMax - (p.y()/yCoeff));
}

QPoint DrawArea::toIntPoint(const QPointF &p) const 
{
	return QPoint(std::round(p.x()), std::round(p.y()));
}

QSize DrawArea::minimumSizeHint () const
{
  return QSize (100, 100);
}

QSize DrawArea::sizeHint () const
{
  return QSize (1000, 1000);
}
