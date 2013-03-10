#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
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
    ~SettingsDialog();
    void loadConfig();

    
private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
