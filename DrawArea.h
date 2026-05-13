#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QResizeEvent>
#include <QPainterPath>

#include "CommonDefs.h"
#include "Approximation.h"
#include "Data.h"

#include <iostream>
#include <cmath>

class Function {
	public:
		std::string name;
		double (*f)(double);
		Function(std::string, double (*)(double));
};

class DrawArea : public QWidget 
{
	Q_OBJECT
private:
	std::vector<Function> functions;

	double a, b;

	QColor bgColor;

	double xMin, xMax, yMin, yMax;
	double xCoeff, yCoeff;

	Data *data;

	QPointF mapToPixels(const Approx::Point &) const;
	Approx::Point mapFromPixels(const QPointF &) const;
	QPoint toIntPoint(const QPointF &) const;

//	bool origin;
//	bool approxFirst;
//	bool approxSecond;
//	bool error;

	bool pointsDrawFlag;
	Approx::Approximator approximator;
public:
	DrawArea(QWidget *, double, double, int, int, Data *);

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void changeFunc();
	void changeType();
	void scaleUp();
	void scaleDown();
	void numberUp();
	void numberDown();
	void errorUp();
	void errorDown();
	void points();

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);

private:
	void recomputeBounds();
	
	void drawCoordSystem(QPainter *);
	void drawPointsFirst(QPainter *);
	void drawPointsSecond(QPainter *);
	
	void drawFunction(QPainter *, double (Approx::Approximator::*)(double)); 
	void drawPolynomOptimized(QPainter *);

	void drawOrigin(QPainter *);
	void drawApproxFirst(QPainter *);
	void drawApproxSecond(QPainter *);
	void drawError(QPainter *);
	
	void drawInformation(QPainter *);

	void initializeMap();
};

#endif // DRAWAREA_H
