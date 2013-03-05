#include <QtGui>
#include <QtCore>
#include "qpanexapp.h"
#include "mainwindow.h"
#include "aboutdialog.h"
#include "ui/addpatient.h"
#include "ui/patientview.h"
#include "ui/settingsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	setupUi(this);
	// setup connections
	// setup services sidebar
	
	setupServiceSideBar();
	setUpConnections();
	QPanexApp::registerMainWindow(this);
}

void MainWindow::setUpConnections()
{
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(action_About, SIGNAL(triggered()), this, SLOT(about()));

	connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
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

// ALL THE SLOTS
void MainWindow::about()
{
	AboutDialog *dlg = new AboutDialog(this);
	dlg->exec();
	dlg->deleteLater();
}

void MainWindow::on_actionDo_something_triggered()
{
    this->statusbar->showMessage(tr("This is it"));
}

void MainWindow::on_actionNew_Patient_triggered()
{

    AddPatient *addPatientView = new AddPatient(frame_2);
    addPatientView->show();
}

void MainWindow::on_actionPatient_View_triggered()
{
    PatientView *patientView = new PatientView(frame_2);
    patientView->show();
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog *settingsDialog = new SettingsDialog(this);
    settingsDialog->exec();
    settingsDialog->deleteLater();
}
