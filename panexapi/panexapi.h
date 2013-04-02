
#ifndef PANEXAPI_H
#define PANEXAPI_H

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include <QNetworkReply>
#include "global_include.h"
#include "form/formpost.h"
#include "patientdataapi.h"
#include "patientapi.h"
#include "serviceapi.h"

class PanexApi : public QObject
{
    Q_OBJECT
public:
    QString authToken;
    QString localDataDir;
    explicit PanexApi(QObject *parent = 0);
    static PanexApi* instance();
    bool SignUpUser(QString userName, QString userPassword, QString userRole,
                    QString userEmail);
    bool LoginUser(QString userEmail, QString userPassword);
    bool EditUser(QString userName, QString userPassword,
                  QString userRole, QString userEmail, QVariantMap savedUserData);
    // Patient Data API Methods
    PatientDataAPI* patientDataAPI();
    PatientAPI* patientAPI();
    ServiceAPI* serviceAPI();

    // Patient Methods
    bool AddPatient(QVariantMap data);
    bool GetPatientList(int page);
    // Apps
    bool UploadApp(QString description, QString name, QString version, QString helpLink, QString thumbnail, QString file, QVariantMap savedUserData);
    bool GetAppList(int page);

    // Services
    bool UploadService(QString description, QString name, QString version, QString helpLink, QString thumbnail, QString file, QString commandLine, QVariantMap savedUserData);

signals:
    void LoginResult(QVariantMap dataMap);
    void SignUpResultSignal(QVariantMap dataMap);
    void EditUserResultSignal(QVariantMap dataMap);
    void AddPatientResultSignal(QVariantMap dataMap);
    void GetPatientListResultSignal(QVariantMap dataList);

    void GenericSignal(QVariantMap dataMap);
    void GenericUploadProgressSignal(qint64 done, qint64 total);


public slots:
private slots:
    void processLoginReply(QNetworkReply* aReply);
    void processSignupReply(QNetworkReply* aReply);
    void processEditUserReply(QNetworkReply* aReply);
    void processAddPatientReply(QNetworkReply* aReply);
    void processGetPatientListReply(QNetworkReply* aReply);
    void processGetAppListReply(QNetworkReply* aReply);

    void GenericSlot(QNetworkReply* aReply);
    void GenericFormPostSlot();
    void uploadProgressGeneric(qint64 done,qint64 total);

private:
    static const QString UrlPanex;
    static const QString UrlUserSignUp;
    static const QString UrlUserLogin;
    static const QString UrlUserEdit;
    static const QString UrlPatientAdd;
    static const QString UrlPatientListGet;
    static const QString UrlAppUpload;
    static const QString UrlAppListGet;
    static const QString UrlServiceUpload;

    FormPostPlugin *formPost;

    static const bool isDebug;

    // Singleton
    static PanexApi* m_panex_api;

};

#endif // PANEXAPI_H
