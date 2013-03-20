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

// XXX: Must be changed when deploying on the server
const QString PanexApi::UrlPanex = "http://localhost:3000";
const QString PanexApi::UrlUserSignUp = PanexApi::UrlPanex + "/users.json";
const QString PanexApi::UrlUserLogin = PanexApi::UrlPanex + "/users/sign_in.json";
const QString PanexApi::UrlUserEdit = PanexApi::UrlPanex + "/users/%1.json"; // http://localhost:3000/users/3.json
const QString PanexApi::UrlPatientAdd = PanexApi::UrlPanex + "/patients.json";
const QString PanexApi::UrlPatientListGet = PanexApi::UrlPanex + "/patients.json";
const QString PanexApi::UrlAppUpload = PanexApi::UrlPanex + "/users/%1/apps.json?auth_token=%2"; ///users/:user_id/apps(.:format)
const QString PanexApi::UrlAppListGet = PanexApi::UrlPanex + "/apps.json";
const QString PanexApi::UrlServiceUpload = PanexApi::UrlPanex + "/users/%1/services.json?auth_token=%2";

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
        dataMap.insert("errorString", "Please check the password, it should be more than 6 characters");
        if (statusCode == 422)
        {
            //            dataMap.insert("errorString", Utils::ConvertMapToString(dataMap["errors"]));
            // For now just keep the verbatim
            dataMap.insert("errorString", "Please check the password, it should be more than 6 characters");
        }
        QLOG_ERROR() << "[PanexAPI] Error in HTTP request" << aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok)
                     << aReply->errorString();
    }
    emit this->EditUserResultSignal(dataMap);
}


bool PanexApi::AddPatient(QVariantMap data)
{
    QUrl url(PanexApi::UrlPatientAdd);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QVariantMap dataMap;
    dataMap.insert("auth_token", this->authToken);
    dataMap.insert("patient", data);

    bool ok;
    QByteArray jsonData = QtJson::serialize(dataMap, ok);
    if (ok)
    {
        QLOG_INFO() << jsonData;
    } else
    {
        QLOG_ERROR() << "Failed to convert data" << jsonData;
    }

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processAddPatientReply(QNetworkReply*)));
    manager->post(request, jsonData);
    return true;
}

void PanexApi::processAddPatientReply(QNetworkReply *aReply)
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
        dataMap.insert("errorString", "An error occured with the request");
        if (statusCode == 422)
        {
            dataMap.insert("errorString", Utils::ConvertMapToString(dataMap["errors"]));
            // For now just keep the verbatim
            //   dataMap.insert("errorString", "Please check the password, it should be more than 6 characters");
        }
        QLOG_ERROR() << "[PanexAPI] Error in HTTP request" << aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok)
                     << aReply->errorString();
    }
    emit this->AddPatientResultSignal(dataMap);
}

// Patient Get List
bool PanexApi::GetPatientList(int page)
{
    QUrl url(PanexApi::UrlPatientListGet);
    url.addQueryItem("auth_token", authToken);
    url.addQueryItem("page", QString(page));

    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processGetPatientListReply(QNetworkReply*)));
    manager->get(request);
    return true;
}

void PanexApi::processGetPatientListReply(QNetworkReply *aReply)
{
    bool ok;
    QLOG_DEBUG() << "[PanexAPI] Network Reply Recieved";

    QByteArray data=aReply->readAll();
    QVariantMap dataMap;
    // TODO: Error Checking
    int statusCode = aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
    if (statusCode == 204 || statusCode == 201 || statusCode == 200 || statusCode == 203)
    {
        dataMap.insert("result", "success");
        QVariantList patientList = QtJson::parse(data, ok).toList();
        if (ok)
        {
            dataMap.insert("patients", patientList);
            QLOG_INFO() << dataMap;
        }
        else
        {
            QLOG_ERROR() << "Error Occured in parsing data" << data;
        }
    }
    else
    {
        // Error
        dataMap.insert("result", "error");
        dataMap.insert("status", aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok));
        dataMap.insert("errorString", aReply->errorString());
        QLOG_ERROR() << "[PanexAPI] Error in HTTP request" << aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok)
                     << aReply->errorString();
    }
    emit this->GetPatientListResultSignal(dataMap);

}

bool PanexApi::UploadApp(QString description, QString name,
                         QString version, QString helpLink,
                         QString thumbnail, QString appFileName,
                         QVariantMap savedUserData)
{
    QString formedUrl = QString(PanexApi::UrlAppUpload)
            .arg(savedUserData["user_id"].toString(), authToken);

    QFileInfo finfo(thumbnail);
    QFileInfo finfo2(appFileName);

    formPost=new FormPostPlugin();

    formPost->setUserAgent("myBrowser"); //sets user agent form on web will recieve - default ""
    formPost->setEncoding("UTF-8"); //sets transfer encoding (default is "utf-8")
    formPost->addField("name", name); // adds fields in name/value pairs
    formPost->addField("description", description);
    formPost->addField("version", version);
    formPost->addField("helpLink", helpLink);

    formPost->addFile("thumbnail", finfo.absoluteFilePath(), "image/jpeg"); //adds file -
    // first arg is form field name (php on server will
    // get it in $_FILES['upload']); second arg is file name on your computer,
    // or it can be QByteArray with file contents,
    // but then you need to specify name as 3rd arg; and 3rd(4th) arg is mime type of file
    formPost->addFile("appFile", finfo2.absoluteFilePath(), "application/zip");

    QNetworkReply* postReply = formPost->postData(formedUrl);
    connect(postReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgressGeneric(qint64,qint64)));

    connect(postReply, SIGNAL(finished()), this, SLOT(GenericFormPostSlot()));

}

void PanexApi::uploadProgressGeneric(qint64 done, qint64 total)
{
    emit this->GenericUploadProgressSignal(done, total);
}

void PanexApi::GenericFormPostSlot()
{
    QByteArray data=formPost->response();
    QString result(data);
    QLOG_DEBUG() << result;
    GenericSlot(formPost->getReplyObject());
}

void PanexApi::GenericSlot(QNetworkReply *aReply)
{
    bool ok;
    QLOG_DEBUG() << "[PanexAPI] Network Reply Recieved";
    QByteArray data=aReply->readAll();
    QString result(data);
    QLOG_DEBUG() << result;
    QVariantMap dataMap;
    // TODO: Error Checking
    int statusCode = aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
    if (statusCode == 204 || statusCode == 201 || statusCode == 200 || statusCode == 203)
    {
        QLOG_DEBUG() << "[PanexAPI] Successful Signal";
        dataMap.insert("result", "success");
    }
    else
    {
        // Error
        dataMap.insert("result", "error");
        dataMap.insert("status", aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok));
        dataMap.insert("errorString", aReply->errorString());
        QLOG_ERROR() << "[PanexAPI] Error in HTTP request" << aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok)
                     << aReply->errorString();
    }

    emit this->GenericSignal(dataMap);
}

bool PanexApi::GetAppList(int page)
{
    QUrl url(PanexApi::UrlAppListGet);
    url.addQueryItem("auth_token", authToken);
    url.addQueryItem("page", QString(page));

    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processGetAppListReply(QNetworkReply*)));
    manager->get(request);
    return true;
}

void PanexApi::processGetAppListReply(QNetworkReply *aReply)
{
    bool ok;
    QLOG_DEBUG() << "[PanexAPI] Network Reply Recieved";

    QByteArray data=aReply->readAll();
    QString replyString(data);
    QVariantMap dataMap;
    // TODO: Error Checking
    int statusCode = aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
    if (statusCode == 204 || statusCode == 201 || statusCode == 200 || statusCode == 203)
    {
        dataMap.insert("result", "success");
        QVariantList appList = QtJson::parse(data, ok).toList();
        if (ok)
        {
            dataMap.insert("apps", appList);
            QLOG_INFO() << replyString;
        }
        else
        {
            QLOG_ERROR() << "Error Occured in parsing data" << data;
        }
    }
    else
    {
        // Error
        dataMap.insert("result", "error");
        dataMap.insert("status", aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok));
        dataMap.insert("errorString", aReply->errorString());
        QLOG_ERROR() << "[PanexAPI] Error in HTTP request" << aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok)
                     << aReply->errorString();
    }
    emit this->GenericSignal(dataMap);

}

bool PanexApi::UploadService(QString description, QString name, QString version,
                            QString helpLink, QString thumbnail, QString serviceFileName,
                            QString commandLine, QVariantMap savedUserData)
{
    QString formedUrl = QString(PanexApi::UrlServiceUpload)
            .arg(savedUserData["user_id"].toString(), authToken);

    QFileInfo finfo(thumbnail);
    QFileInfo finfo2(serviceFileName);

    formPost=new FormPostPlugin();

    formPost->setUserAgent("panex-client");
    formPost->setEncoding("UTF-8");
    formPost->addField("name", name);
    formPost->addField("description", description);
    formPost->addField("commandLine", commandLine);
    formPost->addField("version", version);
    formPost->addField("helpLink", helpLink);

    formPost->addFile("thumbnail", finfo.absoluteFilePath(), "image/jpeg");
    formPost->addFile("serviceFile", finfo2.absoluteFilePath(), "application/zip");

    QNetworkReply* postReply = formPost->postData(formedUrl);
    connect(postReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgressGeneric(qint64,qint64)));

    connect(postReply, SIGNAL(finished()), this, SLOT(GenericFormPostSlot()));
    return true;
}
