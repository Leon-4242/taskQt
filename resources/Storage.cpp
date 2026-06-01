#include "resources/Storage.h"

Storage::Triangle::Triangle (int a, int b, int c, double depth): a(a), b(b), c(c), depth(depth)
{}

Storage::Triangle::Triangle (const Storage::Triangle &tr)
{
	a = tr.a; b = tr.b; c = tr.c; depth = tr.depth;
}
Storage::Triangle::Triangle (Storage::Triangle && tr)
{
	a = tr.a; b = tr.b; c = tr.c; depth = tr.depth;
	tr.a = 0; tr.b = 0; tr.c = 0; tr.depth = 0;
}

Storage::Triangle & Storage::Triangle::operator= (const Storage::Triangle &tr)
{
	a = tr.a; b = tr.b; c = tr.c; depth = tr.depth;
	return *this;
}

Storage::Triangle & Storage::Triangle::operator= (Storage::Triangle && tr)
{
	a = tr.a; b = tr.b; c = tr.c; depth = tr.depth;
	tr.a = 0; tr.b = 0; tr.c = 0; tr.depth = 0;
	return *this;
}

Storage::Storage(QObject *object):
	QObject(object)
{}

Storage::Mesh * Storage::renderrAccess(void)
{
	return &currentFrame;
}

Storage::Mesh * Storage::getAccess(void)
{
	return &newFrame;
}

void Storage::updatedMesh()
{
	readyFrame.swap(newFrame);
	updateable = true;
	emit update();
}

void Storage::updateRenderr()
{
	if (updateable) {
		currentFrame.swap(readyFrame);
		updateable = false;
	}
}
