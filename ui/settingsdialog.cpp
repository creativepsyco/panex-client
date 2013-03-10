#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>

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
    qDebug() << "settings loaded and applied";
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
