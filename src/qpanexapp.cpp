#include "qpanexapp.h"
#include "global_include.h"
#include "mainwindow.h"
#include <QSignalMapper>

QPanexApp::QPanexApp( int & argc, char **argv ) :
        QApplication( argc, argv ),
        m_mainWindow(0)
{
    // Init settings
    // Some config etc. 
    
    // m_core = new Core( this );
    m_settingsDialog = new SettingsDialog;

    // QSignalMapper *mapper = new QSignalMapper( this );
    // mapper->setMapping( qApp, 1 );
    connect( this, SIGNAL(exitApp()), this, SLOT(exitAppSlot()) );
    // connect( mapper, SIGNAL(mapped(int)), m_settingsDialog, SLOT(saveConfig(int)) );
}

QPanexApp::~QPanexApp()
{
    // delete m_settingsDialog;
}

QPanexApp* QPanexApp::instance()
{
    return static_cast<QPanexApp*>( QApplication::instance() );
}

MainWindow* QPanexApp::mainWindow()
{
    return instance()->m_mainWindow;
}

void QPanexApp::registerMainWindow( MainWindow *mainWindow )
{
    if ( !instance()->m_mainWindow ) {
        instance()->m_mainWindow = mainWindow;
    }
}

void QPanexApp::unregisterMainWindow( MainWindow *mainWindow )
{
    if ( instance()->m_mainWindow == mainWindow ) {
        instance()->m_mainWindow = 0;
        // instance()->m_core->storeSession();
    }
}

void QPanexApp::loadConfig()
{
    QPanexApp::settingsDialog() ->loadConfig();
}

void QPanexApp::openSettings()
{
    QPanexApp::settingsDialog()->exec();
}

void QPanexApp::exitAppSlot()
{
    QLOG_INFO() << "Quitting Application";
    // TODO: do something before exit?
    this->exit(0);
    qApp->exit(0);
}

SettingsDialog* QPanexApp::settingsDialog()
{
    return instance()->m_settingsDialog;
}
