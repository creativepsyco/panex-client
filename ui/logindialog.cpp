#include <QMessageBox>
#include "logindialog.h"
#include "qpanexapp.h"
#include "mainwindow.h"
#include "ui_logindialog.h"
#include "usersignupdialog.h"

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
    // Allow Login
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
