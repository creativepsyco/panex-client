#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_lbl_sign_up_linkActivated(const QString &link);

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
