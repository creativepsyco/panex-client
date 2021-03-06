#ifndef QPANEXAPP_H
#define QPANEXAPP_H

#include <QApplication>
#include "ui/settingsdialog.h"
class MainWindow;

class QPanexApp : public QApplication
{
    Q_OBJECT
public:
    static QPanexApp* instance();

    QPanexApp( int & argc, char **argv );
    virtual ~QPanexApp();

    static MainWindow* mainWindow();
    static SettingsDialog* settingsDialog();
    void loadConfig();

signals:
    void exitApp();

public slots:
    void openSettings();
    void exitAppSlot();
    // void loadConfig();

private:
    static void registerMainWindow( MainWindow *mainWindow );
    static void unregisterMainWindow( MainWindow *mainWindow );

    MainWindow *m_mainWindow;
    SettingsDialog *m_settingsDialog;

    friend class MainWindow;
};

#endif // QPANEXAPP_H
