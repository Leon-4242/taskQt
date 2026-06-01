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
