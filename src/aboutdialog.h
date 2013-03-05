#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include "ui_aboutdialog.h"

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    AboutDialog( QWidget *parent = 0 );
    ~AboutDialog();

private:
    QString contributor( const QString &name, const QString &email, const QString &role = QString() );

    void populateAuthors();
    void populateCredits();
    void displayAbout();

    int fontSize;
    QString fontFamily;
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H