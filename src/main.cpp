#include <QApplication>
#include <QWidget>

#include "ui_mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Ui::MainWindow ui;
	QMainWindow *mainWindow = new QMainWindow;
	ui.setupUi(mainWindow);
	mainWindow->show();

	return app.exec();
}