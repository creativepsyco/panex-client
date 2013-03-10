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
    
private slots:
    void on_btn_sign_up_clicked();

private:
    Ui::UserSignupDialog *ui;
};

#endif // USERSIGNUPDIALOG_H
