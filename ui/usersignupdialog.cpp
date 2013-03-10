#include "usersignupdialog.h"
#include "ui_usersignupdialog.h"
#include "logindialog.h"
#include <QMessageBox>
#include "utils.cpp"

UserSignupDialog::UserSignupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserSignupDialog)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    // Removes maximize button
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
}

UserSignupDialog::~UserSignupDialog()
{
    delete ui;
}

void UserSignupDialog::on_btn_sign_up_clicked()
{
    // do the signup

    // Validate as well
    if (validate())
    {
        QMessageBox msgBox;
        msgBox.setText("Signed Up Successfully now login");
        msgBox.exec();
        this->close();
        LoginDialog *loginDialog = new LoginDialog(this->parentWidget());
        loginDialog->exec();
        loginDialog->deleteLater();
    }

}

void UserSignupDialog::on_btn_cancel_clicked()
{
    this->close();
    LoginDialog *loginDialog = new LoginDialog(this->parentWidget());
    loginDialog->exec();
    loginDialog->deleteLater();
}

/* Returns true if validation succeeds */
bool UserSignupDialog::validate()
{
    // Check that none of the fields are empty
    if (ui->txt_email->text().isEmpty()
            || ui->txt_name->text().isEmpty()
            || ui->txt_password->text().isEmpty()
            || ui->txt_confirm_password->text().isEmpty())
    {
        DisplayMessageBox("Incompleted Fields Error","Please Complete all the fields", QMessageBox::Critical);
        return false;
    }
    // Password check
    if (this->ui->txt_password->text() != this->ui->txt_confirm_password->text())
    {
        QMessageBox msgBox;
        msgBox.setText("Please check the password and confirm password match.");
        msgBox.exec();
        return false;
    }

    return true;
}
