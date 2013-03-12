#include "panexapi.h"
#include "global_include.h"
#include "json.h"
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

using namespace QtJson;

const QString PanexApi::UrlPanex = "http://localhost:3000";
const QString PanexApi::UrlUserSignUp = PanexApi::UrlPanex + "/users.json";
const QString PanexApi::UrlUserLogin = PanexApi::UrlPanex + "/users/sign_in.json";


// Need this for singleton purposes
PanexApi* PanexApi::m_panex_api=0;

PanexApi::PanexApi(QObject *parent) :
    QObject(parent)
{
}

PanexApi* PanexApi::instance()
{
    if(PanexApi::m_panex_api)
        return PanexApi::m_panex_api;
    else
    {
        PanexApi::m_panex_api = new PanexApi();
    }
    return PanexApi::m_panex_api;
}

bool PanexApi::SignUpUser(QString userName, QString userPassword, QString userRole, QString userEmail)
{
    QUrl url(PanexApi::UrlUserSignUp);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    //    connect(manager, SIGNAL(finished(QNetworkReply*)),
    //            this, SLOT(syncRequestFinished(QNetworkReply*)));
    QVariantMap dataMap;
    dataMap.insert("name", userName);
    dataMap.insert("password", userPassword);
    dataMap.insert("role", userRole);
    dataMap.insert("email", userEmail);

    QVariantMap userDataMap;
    userDataMap.insert("user", dataMap);

    bool ok;
    QByteArray jsonData = QtJson::serialize(userDataMap, ok);
    if (ok)
    {
        QLOG_INFO() << jsonData;
    } else
    {
        QLOG_ERROR() << "Failed to convert data" << jsonData;
    }

    //    // FIXME for debug
    //    qDebug() << "Sync" << QString::fromUtf8(data.data(), data.size());

    manager->post(request, jsonData);
    return true;
}
