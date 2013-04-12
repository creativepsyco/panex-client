#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QVariantMap>
#include "logindialog.h"
#include "patientview.h"
#include "usersignupdialog.h"
#include <QNetworkAccessManager>
#include "addpatient.h"

class QAction;
class QLabel;
class QPushButton;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public slots:
    void about();
    void loginDialogEvent(bool result);

signals:
    void showLoginDialog();
    void showRegisterDialog();

public:
    MainWindow( QWidget *parent = 0 );
    PatientView* patientViewDialog();

    AddPatient * prepareAddPatientView();
protected:

private slots:

    void on_actionDo_something_triggered();

    void on_actionNew_Patient_triggered();

    void on_actionPatient_View_triggered();

    void on_actionSettings_triggered();

    void on_viewPatientsCommand_clicked();

    void on_actionBack_triggered();

    void on_stackedWidget_currentChanged(int arg1);

    void showLoginDialogBox();
    void showRegisterDialogBox();
    void loginSuccessSlot(QVariantMap aResult);
    void offlineStateChanged(QNetworkAccessManager::NetworkAccessibility access);
    void offlineStateChanged(bool isOnline);

    void on_actionUpload_App_triggered();

    void on_actionExit_triggered();

    void on_actionUpload_Service_triggered();

    void on_actionUpload_Data_triggered();

    void on_actionEdit_Selected_Patient_triggered();

    void on_actionView_Selected_Patient_triggered();

    void on_actionRun_Service_triggered();

    void on_actionView_Server_Status_triggered();

    void on_actionSearch_Patient_triggered();

private:
    void setUpConnections();
    void populateServices(QWidget *widget);
    void setupServiceSideBar();
    void setupStatusBar();
    void clearLayout(QLayout *layout);

    QAction *aboutAction;

    QWidget *servicesSideBar;
    QList<QPushButton *> serviceButtonList;
    LoginDialog *loginDialog;
    UserSignupDialog *userSignupDialog;
    PatientView *patientView;

    int current_widget_id;
    int previous_widget_id;
};

#endif //MAINWINDOW_H
