//
// Copyright (C) 2012-13  Mohit Kanwal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "patientdataapi.h"
#include "form/formpostinterface.h"
#include "global_include.h"
#include "form/formpost.h"
#include "utils.h"
#include "qt-json/json.h"

#include <QFileInfo>
#include <QVector>

using namespace QtJson;

#define ALL_PATIENTS -1

QString PatientDataAPI::UrlPanex = "";
QString PatientDataAPI::UrlPatientDataUpload = "";
QString PatientDataAPI::UrlGetPatientDataListById ="";
QString PatientDataAPI::UrlGetPatientDataList="";
QString PatientDataAPI::UrlDownloadPatientDataFile="";

///
/// \brief PatientDataAPI::PatientDataAPI Constructor
/// \param parent
/// \param rootUrl
///
PatientDataAPI::PatientDataAPI(QObject *parent, QString rootUrl) :
    QObject(parent)
{
    // if you are initializing the URLs here, then remember to declare them
    // at the top of the class
    PatientDataAPI::UrlPanex = rootUrl;
    PatientDataAPI::UrlPatientDataUpload = PatientDataAPI::UrlPanex + "/patients/%1/patient_data/upload.json";
    PatientDataAPI::UrlGetPatientDataList = PatientDataAPI::UrlPanex + "/patients/patient_data.json";
    PatientDataAPI::UrlGetPatientDataListById = PatientDataAPI::UrlPanex + "/patients/%1/patient_data.json";
    PatientDataAPI::UrlDownloadPatientDataFile = PatientDataAPI::UrlPanex + "/patients/%1/patient_data/download/%2.json";
}

///
/// \brief PatientDataAPI::UploadData
/// \param name
/// \param description
/// \param patient_id
/// \param creator_id
/// \param files
/// \return
///
bool PatientDataAPI::UploadData(QString condition, QString description, int patient_id, QString creator_id, QStringList files)
{
    QString formedUrl = QString(PatientDataAPI::UrlPatientDataUpload)
            .arg(patient_id);

    formPost=new FormPostPlugin();

    formPost->setUserAgent("myBrowser"); //sets user agent form on web will recieve - default ""
    formPost->setEncoding("UTF-8"); //sets transfer encoding (default is "utf-8")
    formPost->addField("condition", condition); // adds fields in name/value pairs
    formPost->addField("description", description);
    formPost->addField("creator_id", QString(creator_id));
    formPost->addField("auth_token", PatientDataAPI::authToken);

    int index = 0;
    foreach (QString file, files)
    {
        QFileInfo finfo(file);
        QString str = QString("files[%1]").arg(index);
        formPost->addFile(str, finfo.absoluteFilePath(), Utils::GetMimeType(finfo.suffix()));
        index++;
    }

    QNetworkReply* postReply = formPost->postData(formedUrl);
    connect(postReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgressGeneric(qint64,qint64)));

    connect(postReply, SIGNAL(finished()), this, SLOT(GenericFormPostSlot()));
}

///
/// \brief PatientDataAPI::GenericSlot
/// \param aReply
///
void PatientDataAPI::GenericSlot(QNetworkReply* aReply)
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

///
/// \brief PatientDataAPI::GenericFormPostSlot
///
void PatientDataAPI::GenericFormPostSlot()
{
    QByteArray data=formPost->response();
    QString result(data);
    QLOG_DEBUG() << result;
    GenericSlot(formPost->getReplyObject());
}

///
/// \brief PatientDataAPI::uploadProgressGeneric
/// \param done
/// \param total
///
void PatientDataAPI::uploadProgressGeneric(qint64 done,qint64 total)
{
    emit this->GenericUploadProgressSignal(done, total);
}


bool PatientDataAPI::GetAllPatientDataList()
{
    this->GetPatientDataList(ALL_PATIENTS);
}

///
/// \brief PatientDataAPI::GetPatientDataList
/// \param patientId
/// \return
///
bool PatientDataAPI::GetPatientDataList(int patientId)
{
    QString formedUrl = QString(PatientDataAPI::UrlGetPatientDataListById)
            .arg(patientId);
    if (patientId == ALL_PATIENTS)
    {
        formedUrl = PatientDataAPI::UrlGetPatientDataList;
    }

    QUrl theUrl(formedUrl);
    theUrl.addQueryItem("auth_token", PatientDataAPI::authToken);
    QNetworkRequest request(theUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processGetPatientDataReply(QNetworkReply*)));
    manager->get(request);
    return true;
}

///
/// \brief PatientDataAPI::processGetPatientDataReply
/// \param aReply
///
void PatientDataAPI::processGetPatientDataReply(QNetworkReply *aReply)
{
    bool ok;

    QByteArray data=aReply->readAll();
    //    QLOG_DEBUG() << "[PatientDataAPI] Network Reply Recieved" << data;
    QVariantMap dataMap;
    // TODO: Error Checking
    int statusCode = aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
    if (statusCode == 204 || statusCode == 201 || statusCode == 200 || statusCode == 203)
    {
        dataMap.insert("result", "success");

        QVariantMap patientDataMap = QtJson::parse(data, ok).toMap();
        if (ok)
        {
            dataMap.insert("data", patientDataMap.value("data"));
        }
        else
        {
            QLOG_ERROR() << "[PatientDataAPI] Error Occured in parsing data" << data;
        }
    }
    else
    {
        // Error
        dataMap.insert("result", "error");
        dataMap.insert("status", aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok));
        dataMap.insert("errorString", aReply->errorString());
        QLOG_ERROR() << "[PatientDataAPI] Error in HTTP request" << aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok)
                     << aReply->errorString();
    }
    //    QLOG_DEBUG() << "[PatientDataAPI] Data Recd " << dataMap;
    emit this->GetPatientDataResultSignal(dataMap);
}

bool PatientDataAPI::DownloadPatientDataFile(QString fileId, QString patientId, QString dataType)
{
    QString formedUrl = QString(PatientDataAPI::UrlDownloadPatientDataFile)
            .arg(patientId, fileId);
    QUrl theUrl(formedUrl);

    theUrl.addQueryItem("auth_token", PatientDataAPI::authToken);
    theUrl.addQueryItem("dataType", dataType);

    QNetworkRequest request(theUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processGetDownloadedFileReply(QNetworkReply*)));
    manager->get(request);

    return true;
}

QString PatientDataAPI::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    basename = "/tmp/" + basename;

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}


void PatientDataAPI::processGetDownloadedFileReply(QNetworkReply* aResult)
{
    QString absoluteFilePath = "";
    QString errorString = "";
    if (aResult->error() != QNetworkReply::NoError)
    {
        errorString = aResult->errorString();
        return;
    }
    else
    {
        // Find out the directory and file name
        QString baseName = filenameFromHTTPContentDisposition(aResult->rawHeader("Content-Disposition"));
        QLOG_DEBUG() << "[PatientDataAPI] File Name: " << baseName;
        QFile *file = new QFile("/tmp/" + baseName);
        file->open(QIODevice::WriteOnly);
        file->write(aResult->readAll());
        absoluteFilePath = baseName;
    }

    emit this->PatientDataDownloadFinished(absoluteFilePath, errorString);
}

// FIXME: This function doesn't comply with RFC 6266.
// For example, this function doesn't handle the interaction between " and ;
// that arises from quoted-string, nor does this function properly unquote
// attribute values. Further this function appears to process parameter names
// in a case-sensitive manner. (There are likely other bugs as well.)
QString PatientDataAPI::filenameFromHTTPContentDisposition(QString value)
{
    QStringList keyValuePairs = value.split(';');

    unsigned length = keyValuePairs.size();
    for (unsigned i = 0; i < length; i++) {
        int valueStartPos = keyValuePairs[i].indexOf('=');
        if (valueStartPos < 0)
            continue;

        QString key = keyValuePairs[i].left(valueStartPos).trimmed();

        if (key.isEmpty() || key != "filename")
            continue;

        QString value = keyValuePairs[i].mid(valueStartPos + 1).trimmed();

        // Remove quotes if there are any
        if (value[0] == '\"')
            value = value.mid(1, value.length() - 2);

        return value;
    }

    return QString();
}
