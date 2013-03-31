#include "logindialog.h"
#include "qpanexapp.h"
#include "mainwindow.h"
#include "ui_logindialog.h"
#include "usersignupdialog.h"
#include "global_include.h"
#include <QMessageBox>
#include <QVariantMap>
#include "utils.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    // Removes maximize button
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);

    QVariantMap data;
    data = QPanexApp::instance()->settingsDialog()->getLoginData();
    if(data.contains("email") && data.contains("password"))
    {
        ui->txt_username->setText(data["email"].toString());
        ui->txt_password->setText(data["password"].toString());
    }

    connect(PanexApi::instance(), SIGNAL(LoginResult(QVariantMap)), this, SLOT(processLoginResult(QVariantMap)));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_buttonBox_accepted()
{
    // Do the Login Sequence
    PanexApi *panexApi = PanexApi::instance();

    panexApi->LoginUser(ui->txt_username->text(), ui->txt_password->text());

    // if remember is clicked
    // save to config
    if (ui->checkRememberLogin->isChecked())
    {
        //Save
        QVariantMap data;
        data.insert("email", ui->txt_username->text());
        data.insert("password", ui->txt_password->text());
        QPanexApp::instance()->settingsDialog()->saveLoginData(data);
    }
}

void LoginDialog::on_buttonBox_rejected()
{
    qApp->exit(0);
}

void LoginDialog::on_btn_sign_up_clicked()
{
    this->close();
    emit this->showRegisterDialogSignal();
}

void LoginDialog::show()
{
    this->show();
}

void LoginDialog::processLoginResult(QVariantMap aResult)
{
    QLOG_INFO() << "[Login Dialog] Processing Login Result" << aResult;
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        // emit the correct signal
        emit this->loginSucessSignal(aResult);
        QLOG_DEBUG() << "[Login Dialog] Success in getting reply";
    }
    else
    {
        //this->show();
        QLOG_DEBUG() << "[LoginDialog] Error Recd. Therefore firing show logindialogboxsignal again";
        Utils::DisplayMessageBox(aResult["errorString"].toString(), aResult["message"].toString() , QMessageBox::Information);
        emit this->showLoginBoxSignal();
    }
}
