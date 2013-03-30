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

#include "patientapi.h"
#include "form/formpostinterface.h"
#include "global_include.h"
#include "form/formpost.h"
#include "utils.h"
#include "qt-json/json.h"

#include <QFileInfo>
#include <QVector>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

using namespace QtJson;

QString PatientAPI::UrlPanex = "";
QString PatientAPI::UrlPatientGetInfo = "";
QString PatientAPI::UrlPatientEditInfo = "";

PatientAPI::PatientAPI(QObject *parent, QString rootUrl) :
    QObject(parent)
{
    PatientAPI::UrlPanex = rootUrl;

    PatientAPI::UrlPatientGetInfo = PatientAPI::UrlPanex + "/patients/%1.json";
    PatientAPI::UrlPatientEditInfo = PatientAPI::UrlPanex + "/patients/%1.json";
}

bool PatientAPI::GetPatientInfo(QString patient_id)
{
    QString formedUrl = QString(PatientAPI::UrlPatientGetInfo)
            .arg(patient_id);

    QUrl theUrl(formedUrl);
    theUrl.addQueryItem("auth_token", PatientAPI::authToken);
    QNetworkRequest request(theUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleGetPatientInfoSlot(QNetworkReply*)));
    manager->get(request);
    return true;
}

void PatientAPI::handleGetPatientInfoSlot(QNetworkReply *aReply)
{
    bool ok;
    QByteArray data=aReply->readAll();
    QVariantMap dataMap;
    // TODO: Error Checking
    int statusCode = aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
    if (statusCode == 204 || statusCode == 201 || statusCode == 200 || statusCode == 203)
    {
        dataMap.insert("result", "success");

        QVariantMap patientDataMap = QtJson::parse(data, ok).toMap();
        if (ok)
        {
            dataMap.insert("data", patientDataMap);
        }
        else
        {
            QLOG_ERROR() << "[PatientAPI] Error Occured in parsing data" << data;
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
    emit this->GetPatientInfoResultSignal(dataMap);
}

bool PatientAPI::EditPatient(QVariantMap patientData)
{
    QUrl url(PatientAPI::UrlPatientEditInfo.arg(patientData["id"].toString()));
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QVariantMap dataMap;
    dataMap.insert("auth_token", this->authToken);
    dataMap.insert("patient", patientData);

    bool ok;
    QByteArray jsonData = QtJson::serialize(dataMap, ok);
    if (ok)
    {
        QLOG_INFO() << jsonData;
    } else
    {
        QLOG_ERROR() << "Failed to convert data" << jsonData;
    }

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleEditPatientInfoSlot(QNetworkReply*)));
    manager->put(request, jsonData);
    return true;
}

void PatientAPI::handleEditPatientInfoSlot(QNetworkReply *aReply)
{
    bool ok;
    QByteArray data=aReply->readAll();
    QVariantMap dataMap;
    // TODO: Error Checking
    int statusCode = aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
    if (statusCode == 204 || statusCode == 201 || statusCode == 200 || statusCode == 203)
    {
        dataMap.insert("result", "success");
    }
    else
    {
        // Error
        dataMap.insert("result", "error");
        dataMap.insert("status", aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok));
        dataMap.insert("errorString", aReply->errorString());
        QLOG_ERROR() << "[PatientAPI] Error in HTTP request" << aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok)
                     << aReply->errorString();
    }
    //    QLOG_DEBUG() << "[PatientDataAPI] Data Recd " << dataMap;
    emit this->EditPatientInfoResultSignal(dataMap);
}
