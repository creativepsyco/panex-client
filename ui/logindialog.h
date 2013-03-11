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

signals:
    void showRegisterDialogSignal();

public slots:
    void show();
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_btn_sign_up_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
