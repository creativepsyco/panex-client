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

#ifndef PATIENTDATAUPLOAD_H
#define PATIENTDATAUPLOAD_H

#include <QDialog>
#include <QStringList>
#include <QVariantMap>
#include <QStandardItem>
#include <QList>
#include <QStandardItemModel>

namespace Ui {
class PatientDataUpload;
}

class PatientDataUpload : public QDialog
{
    Q_OBJECT
    
public:
    explicit PatientDataUpload(QWidget *parent = 0);
    ~PatientDataUpload();
    
    void setHeaders();
private slots:
    void on_btnCancel_clicked();

    void on_btnAdd_clicked();
    void removeFiles(QStringList Files);
    void addFiles(QStringList Files);
    QList<QStandardItem *> prepareRow(const QString &first,
                                      const QString &second,
                                      const QString &third);

    void on_btnRemove_clicked();

    void on_btnUpload_clicked();
    void GenericUploadSlot(qint64 done, qint64 total);
    void handleUploadAPIReply(QVariantMap aResult);

private:
    Ui::PatientDataUpload *ui;
    // Standard Item Model to store list
    //Filename, FilePath, FileType
    QStandardItemModel *fileList;
    int m_patient_id;
};

#endif // PATIENTDATAUPLOAD_H
