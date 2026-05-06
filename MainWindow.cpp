#include "MainWindow.h"
#include <locale.h>

MainWindow::MainWindow(double a, double b, int n, int k, QWidget *parent): QMainWindow(parent), a(a), b(b), n(n)
{
    setlocale(LC_ALL, "C");

	central = new QWidget(this);
	data =  new Data;
	drawArea = new DrawArea (this, a, b, n, k, data);

	infoDisplay = new InfoDisplay(this, data);

	infoDisplay->setFixedWidth(200);

	QHBoxLayout *layout = new QHBoxLayout(central);

	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	layout->addWidget(drawArea);
	layout->addWidget(infoDisplay);
	setCentralWidget (central);

	QMenu *fileMenu = menuBar()->addMenu("&File");
	QMenu *funcMenu = menuBar()->addMenu("&Function");

	QAction *changeFuncAction = funcMenu->addAction("Change function", this, &MainWindow::changeFunc);
	changeFuncAction->setShortcut(QKeySequence("Ctrl+0"));

	QAction *changeTypeAction = funcMenu->addAction("Change type", this, &MainWindow::changeType);
	changeTypeAction->setShortcut(QKeySequence("Ctrl+1"));

	QAction *scaleUpAction = funcMenu->addAction("Scale up", this, &MainWindow::scaleUp);
	scaleUpAction->setShortcut(QKeySequence("Ctrl+2"));

	QAction *scaleDownAction = funcMenu->addAction("Scale down", this, &MainWindow::scaleDown);
	scaleDownAction->setShortcut(QKeySequence("Ctrl+3"));

	QAction *numberUpAction = funcMenu->addAction("Number of points up", this, &MainWindow::numberUp);
	numberUpAction->setShortcut(QKeySequence("Ctrl+4"));

	QAction *numberDownAction = funcMenu->addAction("Number of points down", this, &MainWindow::numberDown);
	numberDownAction->setShortcut(QKeySequence("Ctrl+5"));
	
	QAction *errorUpAction = funcMenu->addAction("Error up", this, &MainWindow::errorUp);
	errorUpAction->setShortcut(QKeySequence("Ctrl+6"));

	QAction *errorDownAction = funcMenu->addAction("Error down", this, &MainWindow::errorDown);
	errorDownAction->setShortcut(QKeySequence("Ctrl+7"));
	

	QAction *pointsAction = funcMenu->addAction("On/off &points", this, &MainWindow::points);
	pointsAction->setShortcut(QKeySequence("Ctrl+P"));
	
	QAction *exitAction = fileMenu->addAction("E&xit", this, &QWidget::close);
	exitAction->setShortcut(QKeySequence::Quit);

	setWindowTitle ("Graph");
}

MainWindow::~MainWindow()
{
	delete data;
}

void MainWindow::changeFunc()
{
    drawArea->changeFunc();
}

void MainWindow::changeType()
{
    drawArea->changeType();
}

void MainWindow::scaleUp()
{
	drawArea->scaleUp();
}

void MainWindow::scaleDown()
{
	drawArea->scaleDown();
}

void MainWindow::numberUp()
{
	drawArea->numberUp();
}

void MainWindow::numberDown()
{
	drawArea->numberDown();
}

void MainWindow::errorUp()
{
	drawArea->errorUp();
}

void MainWindow::errorDown()
{
	drawArea->errorDown();
}

void MainWindow::points()
{
	drawArea->points();
}
