#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QVariantMap>
#include "logindialog.h"
#include "usersignupdialog.h"

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

private:
    void setUpConnections();
    void populateServices(QWidget *widget);
    void setupServiceSideBar();
    void clearLayout(QLayout *layout);

    void showSignInWindow();
    QAction *aboutAction;

    QWidget *servicesSideBar;
    QList<QPushButton *> serviceButtonList;
    LoginDialog *loginDialog;
    UserSignupDialog *userSignupDialog;

    int current_widget_id;
    int previous_widget_id;
};

#endif //MAINWINDOW_H
