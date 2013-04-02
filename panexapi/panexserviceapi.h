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

#ifndef PANEXSERVICEAPI_H
#define PANEXSERVICEAPI_H

#include <QObject>
#include "global_include.h"
#include <QNetworkReply>
#include <QVariantList>
#include <QVariantMap>

class PanexServiceAPI : public QObject
{
    Q_OBJECT
public:
    QString authToken;
    QString localDataDir;
    explicit PanexServiceAPI(QObject *parent = 0, QString rootUrl = "");

    bool startService(QString service_id, QString patient_id, QString creator_id, QVariantList fileList);

signals:
    void GenericSignal(QVariantMap result);
public slots:
    void handleGenericNetworkReply(QNetworkReply* aReply);
private:
    static QString UrlPanex;
    static QString UrlServiceRunPost;
};

#endif // PANEXSERVICEAPI_H
