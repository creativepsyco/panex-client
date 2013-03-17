#include "usersignupdialog.h"
#include "ui_usersignupdialog.h"
#include "logindialog.h"
#include "panexapi.h"
#include "global_include.h"
#include <QMessageBox>
#include "utils.h"

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
        PanexApi* instance = PanexApi::instance();
        connect(instance, SIGNAL(SignUpResultSignal(QVariantMap)), this, SLOT(processSignUpResultSlot(QVariantMap)));

        bool result = instance->SignUpUser(ui->txt_name->text(), ui->txt_password->text(), ui->choice_role->currentText(), ui->txt_email->text());

    }
}

void UserSignupDialog::processSignUpResultSlot(QVariantMap aResult)
{
    QLOG_INFO() << "[Signup Dialog] Processing Signup Result" << aResult;
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        Utils::DisplayMessageBox("", "Signed Up Successfully now login", QMessageBox::Information);
        this->close();

        // emit the correct signal
        QLOG_DEBUG() << "[Signup Dialog] Success in getting reply. User Account Created";
        this->close();
        emit this->showLoginBoxSignal();
    }
    else
    {
        //this->show();
        QLOG_DEBUG() << "[SignupDialog] Error Recd.";
        Utils::DisplayMessageBox(aResult["error"].toString(), aResult["errorString"].toString(), QMessageBox::Critical);
    }
}

void UserSignupDialog::on_btn_cancel_clicked()
{
    this->close();
    emit this->showLoginBoxSignal();
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
        Utils::DisplayMessageBox("Incompleted Fields Error","Please Complete all the fields", QMessageBox::Critical);
        return false;
    }
    // Password check
    if (this->ui->txt_password->text() != this->ui->txt_confirm_password->text())
    {
        Utils::DisplayMessageBox("", "Please check the password and confirm password match.", QMessageBox::Critical);
        return false;
    }

    return true;
}
