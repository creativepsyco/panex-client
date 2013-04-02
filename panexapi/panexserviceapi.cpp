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

#include "panexserviceapi.h"
#include "global_include.h"
#include "qt-json/json.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkRequest>
#include <QVariantMap>
#include <QVariantList>

using namespace QtJson;

QString PanexServiceAPI::UrlPanex = "";
QString PanexServiceAPI::UrlServiceRunPost= "";

PanexServiceAPI::PanexServiceAPI(QObject *parent, QString rootUrl) :
    QObject(parent)
{
    PanexServiceAPI::UrlPanex = rootUrl;
    PanexServiceAPI::UrlServiceRunPost = PanexServiceAPI::UrlPanex + "/jobs/service_run.json";
}


bool PanexServiceAPI::startService(QString service_id, QString patient_id, QString creator_id, QVariantList fileList)
{
    QUrl url(PanexServiceAPI::UrlServiceRunPost);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QVariantMap dataMap;
    dataMap.insert("auth_token", this->authToken);
    dataMap.insert("service_id", service_id);
    dataMap.insert("patient_id", patient_id);
    dataMap.insert("creator_id", creator_id);
    dataMap.insert("input", fileList);

    bool ok;
    QByteArray jsonData = QtJson::serialize(dataMap, ok);
    if (ok)
    {
        QLOG_INFO() << jsonData;
    } else
    {
        QLOG_ERROR() << "Failed to convert data" << jsonData;
    }

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleGenericNetworkReply(QNetworkReply*)));
    manager->post(request, jsonData);
    return true;
}

void PanexServiceAPI::handleGenericNetworkReply(QNetworkReply *aReply)
{
    bool ok;
    QLOG_DEBUG() << "[PanexServiceAPI] Network Reply Recieved";

    QVariantMap dataMap;
    // TODO: Error Checking
    int statusCode = aReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
    if (statusCode == 204 || statusCode == 201 || statusCode == 200 || statusCode == 203)
    {
        QLOG_DEBUG() << "[PanexServiceAPI] Successful Signal";
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
