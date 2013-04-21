#include <QtGui>
#include <QtCore>
#include "qpanexapp.h"
#include "mainwindow.h"
#include "aboutdialog.h"

#include "ui/addpatient.h"
#include "ui/patientview.h"
#include "ui/settingsdialog.h"
#include "ui/usersignupdialog.h"
#include "ui/logindialog.h"
#include "ui/uploadappdialog.h"
#include "ui/uploadservicedialog.h"
#include "ui/patientdataupload.h"
#include "ui/servicerundialog.h"
#include "ui/searchpatientdialog.h"
#include "ui/applicationdownloaddialog.h"
#include "ui/servicerunbrowserdialog.h"
#include "systemstatusdialog.h"

#include "global_include.h"
#include <QVariantMap>
#include <QNetworkConfigurationManager>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    // setup connections
    // setup services sidebar
    loginDialog = NULL;
    userSignupDialog = NULL;
    patientView = NULL;

    setupServiceSideBar();
    setUpConnections();
    setupStatusBar();
    QPanexApp::registerMainWindow(this);
    this->hide();
    this->frame->setVisible(false);
    this->frame_3->setVisible(false);

    emit showLoginDialog();
}

void MainWindow::setupStatusBar()
{
    QNetworkConfigurationManager *mgr = new QNetworkConfigurationManager(this);
    connect(mgr, SIGNAL(onlineStateChanged(bool)), this, SLOT(offlineStateChanged(bool)));
    QNetworkAccessManager *manager =new QNetworkAccessManager(this);;
    connect(manager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
            this, SLOT(offlineStateChanged(QNetworkAccessManager::NetworkAccessibility) ));
}

void MainWindow::offlineStateChanged(bool isOnline)
{
    QLOG_INFO() << "[MainWindow] Network might not be accessible";
    if(!isOnline)
    {
        this->statusbar->showMessage("Network is Offline, Cannot reach server");
    }
}

void MainWindow::offlineStateChanged(QNetworkAccessManager::NetworkAccessibility access)
{
    QLOG_INFO() << "[MainWindow] Network might not be accessible";
    if (access== QNetworkAccessManager::NotAccessible)
    {
        this->statusbar->showMessage("Network is Offline, Cannot reach server");
    }
}

void MainWindow::showLoginDialogBox()
{
    QLOG_DEBUG() << "[MainWindow showLoginBox] showing login box";

    QWidget *parent = 0;
    if ( sender() ) {
        parent = qobject_cast<QWidget*>( sender() );
    }

    if (loginDialog != NULL)
    {

    }
    else
    {
        loginDialog = new LoginDialog(parent ? parent : QPanexApp::instance()->activeWindow() );
        connect(loginDialog, SIGNAL(showRegisterDialogSignal()), this, SLOT(showRegisterDialogBox()));
        connect(loginDialog, SIGNAL(showLoginBoxSignal()), this, SLOT(showLoginDialogBox()));
        connect(loginDialog, SIGNAL(loginSucessSignal(QVariantMap)), this, SLOT(loginSuccessSlot(QVariantMap)));
    }

    loginDialog->open();
}

void MainWindow::loginSuccessSlot(QVariantMap aResult)
{
    QLOG_DEBUG() << aResult;
    QPanexApp::settingsDialog()->saveUserConfig(aResult);
    PanexApi::instance()->authToken = aResult["auth_token"].toString();
    PanexApi::instance()->localDataDir = QPanexApp::instance()->settingsDialog()->getLocalStorageLocation();
    QLOG_DEBUG() << "[MainWindow] Result saved to settings config";
    //PanexApi::instance()->GetAppList(0);
    this->on_actionPatient_View_triggered();
    this->show();
}

void MainWindow::showRegisterDialogBox()
{
    QWidget *parent = 0;
    if ( sender() ) {
        parent = qobject_cast<QWidget*>( sender() );
    }

    if (userSignupDialog == NULL)
    {
        userSignupDialog = new UserSignupDialog(parent ? parent : QPanexApp::instance()->activeWindow() );
        connect(userSignupDialog, SIGNAL(showLoginBoxSignal()), this, SLOT(showLoginDialogBox()));
    }

    userSignupDialog->open();
}

void MainWindow::setUpConnections()
{
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(action_About, SIGNAL(triggered()), this, SLOT(about()));
    connect(this, SIGNAL(showLoginDialog()), this, SLOT(showLoginDialogBox()));

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

void MainWindow::loginDialogEvent(bool result)
{
    if(!result)
    {
        // Just Quit the App
        this->close();
    }
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

AddPatient * MainWindow::prepareAddPatientView()
{
    AddPatient *addPatientView = new AddPatient(frame_2);
    // Check if the widget has been added to the stackedWidget
    int index = stackedWidget->indexOf(addPatientView);
    if (index==-1) // Nope Not added yet
        stackedWidget->addWidget(addPatientView);
    // Finally Set the current widget
    stackedWidget->setCurrentWidget(addPatientView);

    return addPatientView;
}

void MainWindow::on_actionNew_Patient_triggered()
{
    AddPatient *addPatientView = prepareAddPatientView();
    addPatientView->op_mode = AddPatient::NEW_MODE;
    addPatientView->show();
}

void MainWindow::on_actionPatient_View_triggered()
{
    if(this->patientView == NULL)
    {
        this->patientView = new PatientView(frame_2);
    }
    int index = stackedWidget->indexOf(patientView);
    if (index==-1) // Nope Not added yet
        stackedWidget->addWidget(patientView);
    // Finally Set the current widget
    stackedWidget->setCurrentWidget(patientView);

    patientView->show();
}

void MainWindow::on_actionSettings_triggered()
{
    QPanexApp::instance()->openSettings();
}

void MainWindow::on_viewPatientsCommand_clicked()
{
    //    clearLayout(frame_2->layout());
    QWidget *viewPatientsView = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(viewPatientsView);
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::rootPath());
    QListView *list = new QListView(this);
    list->setModel(model);
    list->setRootIndex(model->index(QDir::rootPath()));
    gridLayout->addWidget(list);
    int index = stackedWidget->indexOf(viewPatientsView);
    if (index==-1)
        stackedWidget->addWidget(viewPatientsView);
    stackedWidget->setCurrentWidget(viewPatientsView);
//    list->setIconSize(QSize(40,40));
    list->setViewMode(QListView::ListMode);
//    list->setGridSize(QSize(100,100));
    list->show();
}

void MainWindow::on_actionBack_triggered()
{
    if (previous_widget_id != -1)
        stackedWidget->setCurrentIndex(previous_widget_id);
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    previous_widget_id = current_widget_id;
    current_widget_id = arg1;
    QString s = QString("Current: %1 New: %2").arg(stackedWidget->currentIndex()).arg(arg1);
    statusbar->showMessage(s, 2000);
}

PatientView* MainWindow::patientViewDialog()
{
    return this->patientView;
}

void MainWindow::on_actionUpload_App_triggered()
{
    UploadAppDialog* uploadDialog = new UploadAppDialog(this);
    uploadDialog->open();
}

void MainWindow::on_actionExit_triggered()
{
    // Fire Exit
    emit QPanexApp::instance()->exitApp();
}

void MainWindow::on_actionUpload_Service_triggered()
{
    UploadServiceDialog* uploadServiceDialog = new UploadServiceDialog(this);
    uploadServiceDialog->open();
}

void MainWindow::on_actionUpload_Data_triggered()
{
    PatientDataUpload* patientDUp = new PatientDataUpload(this);
    patientDUp->open();
}

void MainWindow::on_actionEdit_Selected_Patient_triggered()
{
    AddPatient *addPatientView = prepareAddPatientView();
    addPatientView->op_mode = AddPatient::EDIT_MODE;
    addPatientView->show();
}

void MainWindow::on_actionView_Selected_Patient_triggered()
{
    AddPatient *addPatientView = prepareAddPatientView();
    addPatientView->op_mode = AddPatient::VIEW_MODE;
    addPatientView->show();
}

void MainWindow::on_actionRun_Service_triggered()
{
    ServiceRunDialog *sRD = new ServiceRunDialog(this);
    sRD->open();
}

void MainWindow::on_actionView_Server_Status_triggered()
{
    SystemStatusDialog* sD = new SystemStatusDialog(this);
    sD->open();
}

void MainWindow::on_actionSearch_Patient_triggered()
{
    SearchPatientDialog* df = new SearchPatientDialog(this);
    df->open();
}

void MainWindow::on_actionDownload_App_triggered()
{
    ApplicationDownloadDialog* ap = new ApplicationDownloadDialog(this);
    ap->open();
}

void MainWindow::on_actionResults_triggered()
{
    ServiceRunBrowserDialog* sp = new ServiceRunBrowserDialog(this);
    sp->open();
}
