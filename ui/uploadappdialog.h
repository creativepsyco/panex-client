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

#ifndef UPLOADAPPDIALOG_H
#define UPLOADAPPDIALOG_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
class UploadAppDialog;
}

class UploadAppDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UploadAppDialog(QWidget *parent = 0);
    ~UploadAppDialog();
    
    bool validate();
private slots:
    void on_btnFileChoose_clicked();
    void handleUploadAPIReply(QVariantMap dataMap);

    void on_btnCancel_clicked();

    void on_btnUpload_clicked();

private:
    Ui::UploadAppDialog *ui;
};

#endif // UPLOADAPPDIALOG_H
