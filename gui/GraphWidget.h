#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QResizeEvent>
#include <QPolygonF>

#include <iostream>
//#include <cmath>

#include "gui/3DRender.h"
#include "gui/Data.h"

#include "utils/CommonDefs.h"

class GraphWidget : public QWidget 
{
	Q_OBJECT
	
	private:
	using Polygon = struct 
	{
		QPointF vertex[4];
		int n;
		double depth;
		QColor color;
		bool visible;
	};

	Mesh *mesh;
	std::vector<Polygon> polygons;
	Data *data;
	QColor bgColor;
	int w, h;
	double dist;
	R3Geometry::Point center;
	Camera camera;
	R3Geometry::Point lightPos;
	PointLight light;
	bool rotating = false;
	QPoint lastMousePos;

	Polygon pointToPixels(const R3Geometry::Point &) const;
	Polygon segToPixels(const R3Geometry::Point &, const R3Geometry::Point &) const;
	Polygon triangleToPixels(const R3Geometry::Point &, const R3Geometry::Point &, const R3Geometry::Point &) const;

	public:
	
	GraphWidget(QWidget *, double, double, double, double, Mesh *, Data *);
	void scaleUp();
	void scaleDown();
	void rotateLeft();
	void rotateRight();
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	protected:
	
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);

	private:

	void drawCoordSystem(QPainter *);
	void drawMesh(QPainter *, const std::vector<Mesh::Triangle> &, const std::vector<R3Geometry::Point> &);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

	void wheelEvent(QWheelEvent *);

	void initializeMap();

	QColor converstion(const PointLight::Color &) const;

	public slots:
	void updated();

	signals:
	void updateRenderr();	
};

#endif // GRAPHWIDGET_H
