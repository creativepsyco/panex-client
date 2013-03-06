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

void MainWindow::clearLayout(QLayout *layout)
{
    for(int i=0; i<layout->children().size(); i++)
    {
        QLayoutItem *item = layout->itemAt(i);
        if (!item) continue;
        if (item->widget()) {
            layout->removeWidget(item->widget());
        } else {
            layout->removeItem(item);
        }
    }
}

void MainWindow::on_actionNew_Patient_triggered()
{
    AddPatient *addPatientView = new AddPatient(frame_2);
    // Check if the widget has been added to the stackedWidget
    int index = stackedWidget->indexOf(addPatientView);
    if (index==-1) // Nope Not added yet
        stackedWidget->addWidget(addPatientView);
    // Finally Set the current widget
    stackedWidget->setCurrentWidget(addPatientView);

    addPatientView->show();
    //    QFileSystemModel *model = new QFileSystemModel;
    //    model->setRootPath(QDir::currentPath());
    //    QTreeView *tree = new QTreeView(frame_2);
    //    tree->setModel(model);
    //    tree->setRootIndex(model->index(QDir::currentPath()));
    //    tree->show();
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

void MainWindow::on_viewPatientsCommand_clicked()
{
//    clearLayout(frame_2->layout());

    QWidget *viewPatientsView = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(viewPatientsView);
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    QTreeView *tree = new QTreeView(this);
    tree->setModel(model);
    tree->setRootIndex(model->index(QDir::currentPath()));
    gridLayout->addWidget(tree);
    int index = stackedWidget->indexOf(viewPatientsView);
    if (index==-1)
        stackedWidget->addWidget(viewPatientsView);
    stackedWidget->setCurrentWidget(viewPatientsView);
    tree->show();
}
