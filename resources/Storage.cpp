#include "resources/Storage.h"

Storage::Storage(QObject *object):
	QObject(object)
{}

Mesh * Storage::renderrAccess(void)
{
	return &currentFrame;
}

Mesh * Storage::getAccess(void)
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
