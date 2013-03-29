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

#ifndef PATIENTDATAAPI_H
#define PATIENTDATAAPI_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include "form/formpost.h"

class PatientDataAPI : public QObject
{
    Q_OBJECT
public:
    QString authToken;
    explicit PatientDataAPI(QObject *parent = 0, QString rootUrl = "");
    
signals:
    void GenericSignal(QVariantMap dataMap);
    void GenericUploadProgressSignal(qint64 done, qint64 total);
    void GetPatientDataResultSignal(QVariantMap dataMap);

public slots:
    bool UploadData(QString condition, QString description, int patient_id, QString creator_id, QStringList files);
    bool GetPatientDataList(int patientId);
    bool GetAllPatientDataList();

private slots:
    void GenericSlot(QNetworkReply* aReply);
    void GenericFormPostSlot();
    void uploadProgressGeneric(qint64 done,qint64 total);

    void processGetPatientDataReply(QNetworkReply* aDataList);

private:
    static QString UrlPanex;
    static QString UrlPatientDataUpload;
    static QString UrlGetPatientDataList;
    static QString UrlGetPatientDataListById;

    FormPostPlugin *formPost;
};

#endif // PATIENTDATAAPI_H
