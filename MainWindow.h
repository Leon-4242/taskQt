#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QHBoxLayout>

#include "CommonDefs.h"
#include "DrawArea.h"
#include "InfoDisplay.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit MainWindow(double, double, int, int, QWidget *parent = nullptr);
        ~MainWindow();

    private:
		QWidget *central;
		DrawArea *drawArea;
		InfoDisplay *infoDisplay;
		Data *data;

		double a;
		double b;
		int n;

    private slots:
        void changeFunc();
		void changeType();
		void scaleUp();
		void scaleDown();
		void numberUp();
		void numberDown();
		void errorUp();
		void errorDown();
		void points();
};

#endif // MAINWINDOW_H
