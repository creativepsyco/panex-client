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

#ifndef __FORMPOSTINTERFACE_H__
#define __FORMPOSTINTERFACE_H__
#include <QByteArray>
#include <QString>
#include <QNetworkReply>

class FormPost
{
  public:
   virtual ~FormPost() {}
    virtual QString userAgent() = 0;
    virtual void setUserAgent(QString agent) = 0;
    virtual QString referer() = 0;
    virtual void setReferer(QString ref) = 0;
    virtual QString encoding() = 0;
    virtual void setEncoding(QString enc) = 0;
    virtual void addField(QString name, QString value) = 0;
    virtual void addFile(QString fieldName, QByteArray file, QString name, QString mime) = 0;
    virtual void addFile(QString fieldName, QString fileName, QString mime) = 0;
    virtual QNetworkReply * postData(QString url) = 0;
    virtual QByteArray response() = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(FormPost,"com.tuckdesign.Plugin.FormPost/1.0");
QT_END_NAMESPACE

#endif
