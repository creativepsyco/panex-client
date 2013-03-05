#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = 0 );

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void about();
private:
	QAction *aboutAction;
	QToolBar *toolBar;
};

#endif //MAINWINDOW_H