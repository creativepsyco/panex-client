#include <QtGui>
#include "qpanexapp.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	setupUi(this);
	// setup connections
	// setup services sidebar
	setupServiceSideBar();
	QPanexApp::registerMainWindow(this);
}

void MainWindow::setupServiceSideBar()
{
	servicesSideBar = new QWidget(frame_3); //this is parent
	
	populateServices(servicesSideBar);
}

void MainWindow::populateServices(QWidget *sidebar)
{
	QVBoxLayout *vbox = new QVBoxLayout(servicesSideBar);
	vbox->setSpacing(1);
	// assume it gets services from somewhere
	int size = 4;
	int i,j,k;
	for (i=0; i<size; i++)
	{
		// set stuff and add to 
		
		QPushButton *p = new QPushButton(tr(QString("Service_%1").arg(i).toStdString().c_str()), sidebar);
		serviceButtonList.append(p);
		vbox->addWidget(p, Qt::AlignVCenter);
	}
	sidebar->setLayout(vbox);
	sidebar->show();
}

