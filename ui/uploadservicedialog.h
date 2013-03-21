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

#ifndef UPLOADSERVICEDIALOG_H
#define UPLOADSERVICEDIALOG_H

#include <QWidget>
#include <QVariantMap>
#include <QDialog>

namespace Ui {
class UploadServiceDialog;
}

class UploadServiceDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UploadServiceDialog(QWidget *parent = 0);
    ~UploadServiceDialog();
    
private slots:
    void on_btnThumbnailChoose_clicked();

    void on_btnServiceFileChoose_clicked();

    void on_btnUpload_clicked();
    void handleUploadAPIReply(QVariantMap dataMap);
    void updateDataTransferProgress(qint64 read, qint64 total);

    void on_btnCancel_clicked();

private:
    Ui::UploadServiceDialog *ui;
    bool isValidated();
};

#endif // UPLOADSERVICEDIALOG_H
