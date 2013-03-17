#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>
#include "QsLog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::loadConfig()
{
    // Read the settings and apply them to the UI
    QLOG_INFO() << "settings loaded and applied";
}

void SettingsDialog::saveConfig(int quitting)
{

}

void SettingsDialog::show()
{

}

void SettingsDialog::checkForUpdate()
{

}

void SettingsDialog::applySettings()
{

}

void SettingsDialog::saveUserConfig(QVariantMap aUserData)
{
    settings.beginGroup("accounts");
    settings.setValue("auth_token", aUserData["auth_token"]);
    settings.setValue("email", aUserData["email"]);
    settings.setValue("user_id", aUserData["user_id"]);
    settings.endGroup();

    settings.sync();
}
