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

#ifndef PATIENTAPI_H
#define PATIENTAPI_H

#include <QObject>
#include <QNetworkReply>
#include <QVariantMap>

class PatientAPI : public QObject
{
    Q_OBJECT
public:
    QString authToken;
    QString localDataDir;
    explicit PatientAPI(QObject *parent = 0, QString rootUrl ="");
    
    // Patient Functions
    bool GetPatientInfo(QString patient_id);
    bool EditPatient(QVariantMap patientData);

signals:
    void GetPatientInfoResultSignal(QVariantMap patientInfo);
    void EditPatientInfoResultSignal(QVariantMap aResult);

public slots:

private slots:
    void handleGetPatientInfoSlot(QNetworkReply* aReply);
    void handleEditPatientInfoSlot(QNetworkReply* aReply);

private:
    static QString UrlPanex;
    static QString UrlPatientGetInfo;
    static QString UrlPatientEditInfo;

};

#endif // PATIENTAPI_H
