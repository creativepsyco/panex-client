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

#ifndef SERVICEAPI_H
#define SERVICEAPI_H

#include <QObject>
#include <QNetworkReply>
#include <QVariantList>
#include <QVariantMap>

class ServiceAPI : public QObject
{
    Q_OBJECT
public:
    QString authToken;
    QString localDataDir;
    explicit ServiceAPI(QObject *parent = 0, QString rootUrl="");
    
    bool startService(QString service_id, QString patient_id, QString creator_id, QVariantList fileList);
    bool getServiceList();

signals:
    void GenericSignal(QVariantMap result);
    void ServiceListSignal(QVariantMap result);

public slots:
    void handleGenericNetworkReply(QNetworkReply* aReply);
    void handleServiceListReply(QNetworkReply* aReply);

private:
    static QString UrlPanex;
    static QString UrlServiceRunPost;
    static QString UrlServiceListGet;
};

#endif // SERVICEAPI_H
