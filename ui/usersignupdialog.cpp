#include "usersignupdialog.h"
#include "ui_usersignupdialog.h"
#include "logindialog.h"
#include <QMessageBox>

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
    QMessageBox msgBox;
    msgBox.setText("Signed Up Successfully now login");
    msgBox.exec();
    this->close();
    LoginDialog *loginDialog = new LoginDialog(this->parentWidget());
    loginDialog->exec();
    loginDialog->deleteLater();

}
