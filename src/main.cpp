#include <QApplication>
#include <QWidget>
#include <QIcon>
#include <QDir>
#include <iostream>

#include "mainwindow.h"
#include "qpanexapp.h"
#include "qslog/QsDebugOutput.h"
#include "qslog/QsLog.h"
#include "qslog/QsLogDest.h"

void setLog(QPanexApp *a)
{
    // init the logging mechanism
    QsLogging::Logger& logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(a->applicationDirPath()).filePath("log.txt"));
    QsLogging::DestinationPtr fileDestination(
       QsLogging::DestinationFactory::MakeFileDestination(sLogPath) );
    QsLogging::DestinationPtr debugDestination(
       QsLogging::DestinationFactory::MakeDebugOutputDestination() );
    logger.addDestination(debugDestination.get());
    logger.addDestination(fileDestination.get());
    //logger.setLoggingLevel(QsLogging::InfoLevel);
}


int main(int argc, char *argv[])
{
    QPanexApp app(argc, argv);
//    setLog(&app);

    // init the logging mechanism (fixed)
    QsLogging::Logger& logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(app.applicationDirPath()).filePath("log.txt"));
    QsLogging::DestinationPtr fileDestination(
       QsLogging::DestinationFactory::MakeFileDestination(sLogPath) );
    QsLogging::DestinationPtr debugDestination(
       QsLogging::DestinationFactory::MakeDebugOutputDestination() );
    logger.addDestination(debugDestination.get());
    logger.addDestination(fileDestination.get());
    //logger.setLoggingLevel(QsLogging::InfoLevel);

    // Log test
    QLOG_INFO() << "Program started";
    QLOG_INFO() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();

    MainWindow qpanex;
    app.loadConfig();



    qApp->setWindowIcon(QIcon(":/icons/Blueprint.png"));
    //qApp->setQuitOnLastWindowClosed(false);

    qpanex.show();

    return app.exec();
}

