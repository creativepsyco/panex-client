#ifndef PANEXAPI_H
#define PANEXAPI_H

#include <QObject>
#include <QVariantMap>
#include <QNetworkReply>
#include "global_include.h"


class PanexApi : public QObject
{
    Q_OBJECT
public:
    explicit PanexApi(QObject *parent = 0);
    static PanexApi* instance();
    bool SignUpUser(QString userName, QString userPassword, QString userRole, QString userEmail);
    bool LoginUser(QString userEmail, QString userPassword);
    // Patient Methods
//    bool AddPatient(QVariantMap data);

signals:
    void LoginResult(QVariantMap dataMap);
    void SignUpResultSignal(QVariantMap dataMap);


public slots:
private slots:
    void processLoginReply(QNetworkReply* aReply);
    void processSignupReply(QNetworkReply* aReply);

private:
    static const QString UrlPanex;
    static const QString UrlUserSignUp;
    static const QString UrlUserLogin;
    static const bool isDebug;

    // Singleton
    static PanexApi* m_panex_api;
};

#endif // PANEXAPI_H
