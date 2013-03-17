#include "panexapi.h"
#include "global_include.h"
#include "json.h"
#include "utils.h"
#include <QtNetwork>
#include <QVariantMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

using namespace QtJson;

const QString PanexApi::UrlPanex = "http://localhost:3000";
const QString PanexApi::UrlUserSignUp = PanexApi::UrlPanex + "/users.json";
const QString PanexApi::UrlUserLogin = PanexApi::UrlPanex + "/users/sign_in.json";
const QString PanexApi::UrlUserEdit = PanexApi::UrlPanex + "/users/%1.json"; // http://localhost:3000/users/3.json


// Need this for singleton purposes
PanexApi* PanexApi::m_panex_api=0;

PanexApi::PanexApi(QObject *parent) :
    QObject(parent)
{
    this->authToken = "";
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

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processSignupReply(QNetworkReply*)));
    manager->post(request, jsonData);
    return true;
}

void PanexApi::processSignupReply(QNetworkReply* aReply)
{
    bool ok;
    QLOG_DEBUG() << "[PanexAPI] Network Reply Recieved";
    QByteArray data=aReply->readAll();
    int statusCode = aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
    if (ok && (statusCode == 201 || statusCode==200))
    {
        // Success
        QVariantMap dataMap = QtJson::parse(data, ok).toMap();

        dataMap.insert("result", "success");
        emit this->SignUpResultSignal(dataMap);
    }
    else
    {
        QVariantMap dataMap = QtJson::parse(data, ok).toMap();
        // Handle any errors here:
        dataMap.insert("result", "error");
        dataMap.insert("status", aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok));
        dataMap.insert("errorString", aReply->errorString());

        if (statusCode == 422)
        {
            dataMap.insert("errorString", Utils::ConvertMapToString(dataMap["errors"]));
        }

        QLOG_ERROR() << "[PanexAPI] Error in HTTP request" << aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok)
                     << Utils::ConvertMapToString(dataMap["errors"]);
        emit this->SignUpResultSignal(dataMap);
    }
}

bool PanexApi::LoginUser(QString userEmail, QString userPassword)
{
    QUrl url(PanexApi::UrlUserLogin);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QVariantMap dataMap;
    dataMap.insert("email", userEmail);
    dataMap.insert("password", userPassword);

    bool ok;
    QByteArray jsonData = QtJson::serialize(dataMap, ok);
    if (ok)
    {
        QLOG_INFO() << jsonData;
    } else
    {
        QLOG_ERROR() << "Failed to convert data" << jsonData;
    }

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processLoginReply(QNetworkReply*)));
    manager->post(request, jsonData);
    return true;
}

void PanexApi::processLoginReply(QNetworkReply* aReply)
{
    bool ok;
    QLOG_DEBUG() << "[PanexAPI] Network Reply Recieved";
    QByteArray data=aReply->readAll();


    // TODO: Error Checking
    int statusCode = aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
    if (ok && statusCode == 200)
    {
        // Success
        QVariantMap dataMap = QtJson::parse(data, ok).toMap();

        dataMap.insert("result", "success");
        emit this->LoginResult(dataMap);
    }
    else
    {
        // Error
        QVariantMap dataMap = QtJson::parse(data, ok).toMap();
        dataMap.insert("result", "error");
        dataMap.insert("status", aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok));
        dataMap.insert("errorString", aReply->errorString());

        QLOG_ERROR() << "[PanexAPI] Error in HTTP request" << aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok)
                     << aReply->errorString();

        emit this->LoginResult(dataMap);
    }
}

bool PanexApi::EditUser(QString userName, QString userPassword, QString userRole, QString userEmail, QVariantMap savedUserData)
{
    QString formedUrl = QString(PanexApi::UrlUserEdit).arg(savedUserData["user_id"].toString());
    QUrl url(formedUrl);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QVariantMap dataMap;
    if (!userEmail.isEmpty()) dataMap.insert("email", userEmail);
    if (!userPassword.isEmpty()) dataMap.insert("password", userPassword);
    if (!userName.isEmpty()) dataMap.insert("name", userName);
    if (!userRole.isEmpty()) dataMap.insert("role", userRole);

    QVariantMap userDataMap;
    userDataMap.insert("user", dataMap);
    userDataMap.insert("auth_token", this->authToken);

    bool ok;
    QByteArray jsonData = QtJson::serialize(userDataMap, ok);
    if (ok)
    {
        QLOG_INFO() << jsonData;
    } else
    {
        QLOG_ERROR() << "Failed to convert data" << jsonData;
    }

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processEditUserReply(QNetworkReply*)));
    manager->put(request, jsonData);
    return true;
}

void PanexApi::processEditUserReply(QNetworkReply *aReply)
{
    bool ok;
    QLOG_DEBUG() << "[PanexAPI] Network Reply Recieved";


    QByteArray data=aReply->readAll();
    QVariantMap dataMap = QtJson::parse(data, ok).toMap();

    // TODO: Error Checking
    int statusCode = aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
    if (statusCode == 204 || statusCode == 201 || statusCode == 200 || statusCode == 203)
    {
        dataMap.insert("result", "success");
    }
    else
    {
        // Error
        QVariantMap dataMap;

        dataMap.insert("result", "error");
        dataMap.insert("status", aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok));
        dataMap.insert("errorString", aReply->errorString());
        if (statusCode == 422)
        {
            dataMap.insert("errorString", Utils::ConvertMapToString(dataMap["errors"]));
            // For now just keep the verbatim
            dataMap.insert("errorString", "Please check the password, it should be more than 6 characters");
        }
        QLOG_ERROR() << "[PanexAPI] Error in HTTP request" << aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok)
                     << aReply->errorString();
    }
    emit this->EditUserResultSignal(dataMap);
}
