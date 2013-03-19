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

#ifndef __FORMPOST_H__
#define __FORMPOST_H__
#include <QtPlugin>
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QProgressBar>
#include <QDateTime>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "formpostinterface.h";

class FormPostPlugin: public QWidget, FormPost
{
Q_OBJECT
Q_INTERFACES(FormPost)
  public:
    FormPostPlugin();
    QString userAgent();
    void setUserAgent(QString agent);
    QString referer();
    void setReferer(QString ref);
    QString encoding();
    void setEncoding(QString enc);
    void addField(QString name, QString value);
    void addFile(QString fieldName, QByteArray file, QString name, QString mime);
    void addFile(QString fieldName, QString fileName, QString mime);
    QNetworkReply * postData(QString url);
    QNetworkAccessManager * postDataWithNetwork(QString url);
    QByteArray response();
  private:
    QByteArray data;
    QByteArray strToEnc(QString s);
    QString encodingS;
    QString userAgentS;
    QString refererS;
    QStringList fieldNames;
    QStringList fieldValues;
    QStringList fileFieldNames;
    QStringList fileNames;
    QStringList fileMimes;
    QList<QByteArray> files;
  private slots:
    void readData(QNetworkReply * r);
};

#endif
