#include "usersignupdialog.h"
#include "ui_usersignupdialog.h"

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
