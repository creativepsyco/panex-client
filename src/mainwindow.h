#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"

#include <QMainWindow>

class QAction;
class QLabel;
class QPushButton;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = 0 );

protected:    

private slots:
    
private:
	void populateServices(QWidget *widget);
	void setupServiceSideBar();
	QAction *aboutAction;

	QWidget *servicesSideBar;
	QList<QPushButton*> serviceButtonList;
};

#endif //MAINWINDOW_H