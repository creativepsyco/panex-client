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

#ifndef SERVICERUNDIALOG_H
#define SERVICERUNDIALOG_H

#include <QDialog>
#include <QVariantMap>
#include <QVariantList>
#include <QVariant>
#include <QStandardItemModel>

namespace Ui {
class ServiceRunDialog;
}

class ServiceRunDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ServiceRunDialog(QWidget *parent = 0);
    void setInputList(QVariantList theList);
    ~ServiceRunDialog();
    
private slots:
    void on_btnCancel_clicked();

    void on_btnStart_clicked();
    void handleServiceListReply(QVariantMap aResult);
    void handleGenericAPIReply(QVariantMap aResult);

private:
    Ui::ServiceRunDialog *ui;
    QVariantList inputFiles;
    QString patient_id;
    QStandardItemModel *serviceListModel;

    void setUpListHeaders();
};

#endif // SERVICERUNDIALOG_H
