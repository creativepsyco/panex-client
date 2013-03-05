#include "aboutdialog.h"
// to be replaced
#define APP_VERSION 1

AboutDialog::AboutDialog( QWidget *parent ) :
        QDialog( parent ),
        ui( new Ui::AboutDialog )
{
    ui->setupUi( this );
    fontSize = font().pointSize();
    fontFamily = font().family();
    populateAuthors();
    populateCredits();
    displayAbout();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

QString AboutDialog::contributor( const QString &name, const QString &email, const QString &role )
{
    if ( role.isNull() )
        return QString( "<p><b>%1</b> - <a href=\"mailto:%2\">%2</a></p>" ).arg( name, email );
    return QString( "<p><b>%1</b> - <a href=\"mailto:%2\">%2</a></p>"
                    "<p>%3</p>"
                    "<p style=\"-qt-paragraph-type:empty; \"></p>" ).arg( name, email, role );
}

void AboutDialog::populateAuthors()
{
    QString authorsHtml( QString( "<html>"
                                  "<head>"
                                  "<style type=\"text/css\">p, li { white-space: pre-wrap; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; }</style>"
                                  "</head>"
                                  "<body style=\" font-family:'%1'; font-size:%2pt; font-weight:400; font-style:normal;\">" ).arg(fontFamily, QString::number(fontSize)) );

    // Populate Authors
    authorsHtml.append( contributor( "Mohit Kanwal", "mohit.kanwal@gmail.com", "Main Tester and Developer" ) );
    authorsHtml.append(   "</body>"
                          "</html>" );

    ui->authorsText->setHtml( authorsHtml );
    ui->authorsText->moveCursor( QTextCursor::Start );
}

void AboutDialog::populateCredits()
{
    QString creditsHtml( QString( "<html>"
                                  "<head>"
                                  "<style type=\"text/css\">p, li { white-space: pre-wrap; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; }</style>"
                                  "</head>"
                                  "<body style=\" font-family:'%1'; font-size:%2pt; font-weight:400; font-style:normal;\">" ).arg(fontFamily, QString::number(fontSize)) );
    creditsHtml.append( contributor( "Mohit Kanwal", "mohit.kanwal@gmail.com", "Main Tester and Developer" ) );

    creditsHtml.append(   "</body>"
                          "</html>" );

    ui->creditsText->setHtml( creditsHtml );
    ui->creditsText->moveCursor( QTextCursor::Start );
}

void AboutDialog::displayAbout()
{
    QString html( QString(  "<html>"
                            "<head>"
                            "<style type=\"text/css\">p, li { white-space: pre-wrap; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; }</style>"
                            "</head>"
                            "<body align=\"center\" style=\" font-family:'%1'; font-size:%2pt; font-weight:400; font-style:normal;\">" ).arg(fontFamily, QString::number(fontSize)) );

    html.append( tr( "<p>Panex - a Qt-based Dektop client for the Panex Webservice</p>"
                     "<p>version %1</p>"
                     "<p>Copyright &copy; 2012-2013</p>"
                     "<p>by <a href=\"http://twitter.com/mohitkanwal\"><span style=\" text-decoration: underline; color:#0000ff;\">Mohit Kanwal</span></a></p>"
                     "<p style=\"-qt-paragraph-type:empty; \"></p>"
                     "<p>Distributed under the LGPL license</p>"
                     "<p>version 2.1 or later</p>" ).arg( APP_VERSION ) );

    html.append(   "</body>"
                   "</html>" );

    ui->aboutText->setHtml( html );
}