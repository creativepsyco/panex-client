#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include<QVariantMap>
#include "configfile.h"

namespace Ui {
class SettingsDialog;
}

extern ConfigFile settings;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public slots:
    void saveConfig( int quitting = 0 );
    void checkForUpdate();
    void show();
    void applySettings();

public:
    explicit SettingsDialog(QWidget *parent = 0);
    void saveUserConfig(QVariantMap aUserData);
    void saveLoginData(QVariantMap aLoginData);
    QVariantMap getUserData();
    QVariantMap getLoginData();
    ~SettingsDialog();
    void loadConfig();
    QString getLocalStorageLocation();

    
private slots:
    void on_btnChangePassword_clicked();

    void on_buttonBox_accepted();

private:
    QString defaultDataStorageLocation;
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
