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

public slots:
    void about();

public:
    MainWindow( QWidget *parent = 0 );

protected:

private slots:

    void on_actionDo_something_triggered();

    void on_actionNew_Patient_triggered();

    void on_actionPatient_View_triggered();

private:
    void setUpConnections();
    void populateServices(QWidget *widget);
    void setupServiceSideBar();
    QAction *aboutAction;

    QWidget *servicesSideBar;
    QList<QPushButton *> serviceButtonList;
};

#endif //MAINWINDOW_H
