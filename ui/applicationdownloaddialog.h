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

#ifndef APPLICATIONDOWNLOADDIALOG_H
#define APPLICATIONDOWNLOADDIALOG_H

#include <QDialog>
#include <QVariantMap>
#include <QStandardItem>
#include <QStandardItemModel>

namespace Ui {
class ApplicationDownloadDialog;
}

class ApplicationDownloadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ApplicationDownloadDialog(QWidget *parent = 0);
    ~ApplicationDownloadDialog();
    
    void setupTreeView(QVariantList appList);
    void setupHeaders();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void HandleAppListAPIReply(QVariantMap dataMap);
private:
    Ui::ApplicationDownloadDialog *ui;
    void updateStatusBar(QString msg);
    QStandardItemModel *appListModel;
};

#endif // APPLICATIONDOWNLOADDIALOG_H
