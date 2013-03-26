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
#include <QFileInfo>


QString PatientDataAPI::UrlPanex = "";
QString PatientDataAPI::UrlPatientDataUpload = "";
///
/// \brief PatientDataAPI::PatientDataAPI Constructor
/// \param parent
/// \param rootUrl
///
PatientDataAPI::PatientDataAPI(QObject *parent, QString rootUrl) :
    QObject(parent)
{
    PatientDataAPI::UrlPanex = rootUrl;
    PatientDataAPI::UrlPatientDataUpload = PatientDataAPI::UrlPanex + "/patients/%1/patient_data/upload.json";
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
bool PatientDataAPI::UploadData(QString name, QString description, int patient_id, QString creator_id, QStringList files)
{
    QString formedUrl = QString(PatientDataAPI::UrlPatientDataUpload)
            .arg(patient_id);

    formPost=new FormPostPlugin();

    formPost->setUserAgent("myBrowser"); //sets user agent form on web will recieve - default ""
    formPost->setEncoding("UTF-8"); //sets transfer encoding (default is "utf-8")
    formPost->addField("name", name); // adds fields in name/value pairs
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

void PatientDataAPI::GenericFormPostSlot()
{
    QByteArray data=formPost->response();
    QString result(data);
    QLOG_DEBUG() << result;
    GenericSlot(formPost->getReplyObject());
}

void PatientDataAPI::uploadProgressGeneric(qint64 done,qint64 total)
{
     emit this->GenericUploadProgressSignal(done, total);
}



