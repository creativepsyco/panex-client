#include <QApplication>
#include <QWidget>
#include <QIcon>

#include "mainwindow.h"
#include "qpanexapp.h"


int main(int argc, char *argv[])
{
    QPanexApp app(argc, argv);
    MainWindow qpanex;
    app.loadConfig();

    qApp->setWindowIcon(QIcon(":/icons/Blueprint.png"));
    //qApp->setQuitOnLastWindowClosed(false);

    qpanex.show();

    return app.exec();
}
