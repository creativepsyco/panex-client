#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"

#include <QMainWindow>

class QAction;
class QLabel;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = 0 );

protected:    

private slots:
    
private:

	QAction *aboutAction;
};

#endif //MAINWINDOW_H