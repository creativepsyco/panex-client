#include "changepassword.h"
#include "global_include.h"
#include "settingsdialog.h"
#include "utils.h"
#include "qpanexapp.h"
#include "ui_changepassword.h"
#include <QVariantMap>

ChangePassword::ChangePassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);
}

ChangePassword::~ChangePassword()
{
    delete ui;
}

void ChangePassword::on_btnCancel_clicked()
{
    this->reject();
}

void ChangePassword::on_btnOK_clicked()
{
    // First get the user Settings
    QVariantMap userData = QPanexApp::settingsDialog()->getUserData();
    // Check if user Password is same as confirm
    if (ui->txtNewPassword->text().compare(ui->txtConfirmNewPassword->text())!=0)
    {
        Utils::DisplayMessageBox("Error Encountered", "Please make sure the passwords match", QMessageBox::Critical);
        return;
    }
    connect(PanexApi::instance(), SIGNAL(EditUserResultSignal(QVariantMap)), this, SLOT(handleApiReply(QVariantMap)));
    PanexApi::instance()->EditUser("",ui->txtNewPassword->text(), "", "", userData);
}

void ChangePassword::handleApiReply(QVariantMap aReply)
{
    QString result = aReply["result"].toString();
    if(result.compare("success") == 0)
    {
        Utils::DisplayMessageBox("Password Changed Successfully", "", QMessageBox::Information);
        this->accept(); // Close this box
    }
    else
    {
        Utils::DisplayMessageBox("Error Encountered", "Please check that the passwords match and that it is more than 6 chars in length", QMessageBox::Information);
    }
}
