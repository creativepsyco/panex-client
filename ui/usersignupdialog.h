#ifndef USERSIGNUPDIALOG_H
#define USERSIGNUPDIALOG_H

#include <QDialog>

namespace Ui {
class UserSignupDialog;
}

class UserSignupDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserSignupDialog(QWidget *parent = 0);
    ~UserSignupDialog();
signals:
    void showLoginBoxSignal();

private slots:
    void on_btn_sign_up_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::UserSignupDialog *ui;
    bool validate();
};

#endif // USERSIGNUPDIALOG_H
