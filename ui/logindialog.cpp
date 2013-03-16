#include <QMessageBox>
#include "logindialog.h"
#include "qpanexapp.h"
#include "mainwindow.h"
#include "ui_logindialog.h"
#include "usersignupdialog.h"
#include "global_include.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    // Removes maximize button
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_buttonBox_accepted()
{
    // Do the Login Sequence
    PanexApi *panexApi = PanexApi::instance();
    // TODO: Validation
    connect(panexApi, SIGNAL(LoginResult(QVariantMap)), this, SLOT(processLoginResult(QVariantMap)));
    panexApi->LoginUser(ui->txt_username->text(), ui->txt_password->text());
}

void LoginDialog::on_buttonBox_rejected()
{
//    QMessageBox msgBox;
//    msgBox.setText("The document has been modified.");
//    msgBox.exec();
//    // The main Window runs as a separate app
//    QApplication::quit();
//    qApp->exit(0);
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
    QLOG_INFO() << "[Login Dialog] Processing Login Result" << aResult["result"].toString();
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        // emit the correct signal
        QLOG_DEBUG() << "[Login Dialog] Success in getting reply";
    }
    else
    {
        this->show();
    }
}
