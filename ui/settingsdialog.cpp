#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "changepassword.h"
#include <QDebug>
#include <QLineEdit>
#include "QsLog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->defaultDataStorageLocation = "panex_client";
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::loadConfig()
{
    // Read the settings and apply them to the UI
    settings.beginGroup("accounts");
    ui->lblUserName->setText(settings.value("email").toString());
    ui->lblAuthToken->setText(settings.value("auth_token").toString());
    settings.endGroup();

    // General
    settings.beginGroup("general");
    if (settings.contains("localStorageDir"))
    {
        ui->txtLocalStorage->setText(settings.value("localStorageDir").toString());
    }
    else
    {
        ui->txtLocalStorage->setText(this->defaultDataStorageLocation);
    }
    settings.endGroup();

    QLOG_INFO() << "[SettingsDialog] settings loaded and applied";
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

QVariantMap SettingsDialog::getUserData()
{
    // TODO: Handle Empty values
    QVariantMap userData;
    settings.beginGroup("accounts");
    userData.insert("auth_token", settings.value("auth_token"));
    userData.insert("email", settings.value("email"));
    userData.insert("user_id", settings.value("user_id"));
    settings.endGroup();
    return userData;
}

void SettingsDialog::saveLoginData(QVariantMap aLoginData)
{
    settings.beginGroup("accounts");
    settings.setValue("email", aLoginData["email"].toString());
    settings.setValue("password", aLoginData["password"].toString());
    settings.endGroup();

    settings.sync();
}

QVariantMap SettingsDialog::getLoginData()
{
    QVariantMap loginData;
    settings.beginGroup("accounts");
    loginData.insert("email", settings.value("email"));
    loginData.insert("password", settings.value("password"));
    settings.endGroup();

    return loginData;
}

void SettingsDialog::on_btnChangePassword_clicked()
{
    ChangePassword *changePasswordDialog = new ChangePassword(this);
    changePasswordDialog->open();
}

void SettingsDialog::on_buttonBox_accepted()
{
    settings.beginGroup("general");
    if (ui->txtLocalStorage->text().length()>0)
    {
        settings.setValue("localStorageDir", ui->txtLocalStorage->text());
    }
    settings.endGroup();
    settings.sync();
}

QString SettingsDialog::getLocalStorageLocation()
{
    return settings.value("general.localStorageDir", this->defaultDataStorageLocation).toString();
}
