#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QVariantMap>
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
    void showLoginBoxSignal();
    void loginSucessSignal(QVariantMap aResult);

public slots:
    void show();
    
private slots:
    void on_buttonBox_accepted();
    void processLoginResult(QVariantMap aResult);
    void on_buttonBox_rejected();

    void on_btn_sign_up_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
