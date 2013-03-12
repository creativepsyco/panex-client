#include "panexapi.h"
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

const QString PanexApi::UrlPanex = "http://localhost:3000";
const QString PanexApi::UrlUserSignUp = PanexApi::UrlPanex + "/users/sign_up.json";
const QString PanexApi::UrlUserLogin = PanexApi::UrlPanex + "/users/sign_in.json";


// Need this for singleton purposes
PanexApi* PanexApi::m_panex_api=0;

PanexApi::PanexApi(QObject *parent) :
    QObject(parent)
{
}

PanexApi* PanexApi::instance()
{
    return PanexApi::m_panex_api;
}

bool PanexApi::SignUpUser(QString userName, QString userPassword, QString userRole, QString userEmail)
{
    QNetworkAccessManager networkManager;
    QUrl url(PanexApi::UrlUserSignUp);
//    QUrl url(SYNC_URL);
//    QNetworkRequest request(url);

//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

//    connect(manager, SIGNAL(finished(QNetworkReply*)),
//            this, SLOT(syncRequestFinished(QNetworkReply*)));

//    QByteArray data = QtJson::Json::serialize(collectSyncData());

//    // FIXME for debug
//    qDebug() << "Sync" << QString::fromUtf8(data.data(), data.size());

//    manager->post(request, data);
    return true;
}
