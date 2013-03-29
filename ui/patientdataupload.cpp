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

#include "patientdataupload.h"
#include "patientdataapi.h"
#include <QFileDialog>
#include <QStringList>
#include <QTreeView>
#include <QVariantMap>
#include "ui_patientdataupload.h"
#include "global_include.h"
#include "qpanexapp.h"
#include "mainwindow.h"

void PatientDataUpload::setHeaders()
{
    QStringList labels;
    labels << "File Name" << "File Type" << "File Path";
    this->fileList->setHorizontalHeaderLabels(labels);
}


PatientDataUpload::PatientDataUpload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientDataUpload)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->txtStatus->setText("");

    this->fileList = new QStandardItemModel;
    this->m_patient_id = 1;

    setHeaders();

    // For test
    //    QList<QStandardItem *> preparedRow =prepareRow("first", "second", "third");
    //    this->fileList->appendRow(preparedRow);
    //    QList<QStandardItem *> secondRow =prepareRow("111", "222", "333");
    //    this->fileList->appendRow(secondRow);
    // End of Test
    ui->treeFileList->setModel(this->fileList);
    ui->treeFileList->expandAll();

}

PatientDataUpload::~PatientDataUpload()
{
    delete ui;
}

void PatientDataUpload::open()
{
    this->m_patient_id = "";
    if (QPanexApp::instance()->mainWindow()->patientViewDialog())
        this->m_patient_id = QPanexApp::instance()->mainWindow()->patientViewDialog()->patient_id;

    QLOG_DEBUG() << "[PatientDataUpload][PatientDataUpload] " << " Patient ID " << this->m_patient_id << " initialized";

    if (this->m_patient_id.length() == 0)
    {
        // Should not show the Dialog
        Utils::DisplayMessageBox("Operation DisAllowed",
                                 "This operation is not allowed. Please select a patient first",
                                 QMessageBox::Critical);
        return;
    }
    QDialog::open();
}

void PatientDataUpload::on_btnCancel_clicked()
{
    // Handle Upload from here
    this->reject();
}

void PatientDataUpload::on_btnAdd_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    // Set Filter ?
    QStringList fileNames;
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
    }

    if(fileNames.length() > 0)
    {
        // Files have been added
        addFiles(fileNames);
    }
}

void PatientDataUpload::addFiles(QStringList Files)
{
    foreach(QString file, Files)
    {
        // Add Standard Item
        QFileInfo finfo(file);
        QList<QStandardItem *> preparedRow = prepareRow(finfo.fileName(),
                                                        finfo.suffix(),
                                                        finfo.absoluteFilePath());
        this->fileList->appendRow(preparedRow);
    }
}

void PatientDataUpload::removeFiles(QStringList Files)
{
    QModelIndexList indices = ui->treeFileList->selectionModel()->selectedIndexes();
    foreach ( QModelIndex index, indices)
    {
        if(index.row() == 0)
        {

        }
        else
        {
            this->fileList->removeRow(index.row(), index.parent());
        }
    }
}

void PatientDataUpload::on_btnRemove_clicked()
{
    QStringList selectedFiles;
    this->removeFiles(selectedFiles);
}

// HELPERS
QList<QStandardItem*> PatientDataUpload::prepareRow(const QString &first,
                                                    const QString &second,
                                                    const QString &third)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(first);
    rowItems << new QStandardItem(second);
    rowItems << new QStandardItem(third);
    return rowItems;
}


/**
 * @brief PatientDataUpload::on_btnUpload_clicked
 *          handle Data uploads for the patient, expects a patient_id
 */
void PatientDataUpload::on_btnUpload_clicked()
{

    bool ok;
    // XXX: Catch the conversion error
    QVariantMap userData = QPanexApp::instance()->settingsDialog()->getUserData();

    // Make a StringList of the various files
    QStringList files;
    for (int rowIndex =0; rowIndex <  this->fileList->rowCount(); rowIndex++)
    {
        QStandardItem *theItem = this->fileList->item(rowIndex, 2);
        files.append(theItem->text());
    }

    PatientDataAPI *instance = PanexApi::instance()->patientDataAPI();
    instance->UploadData("",
                         ui->txtDescription->toPlainText(),
                         this->m_patient_id.toInt(&ok, 10),
                         userData["user_id"].toString(),
            files);
    connect(instance, SIGNAL(GenericUploadProgressSignal(qint64,qint64)), this, SLOT(GenericUploadSlot(qint64,qint64)));
    connect(instance, SIGNAL(GenericSignal(QVariantMap)), this, SLOT(handleUploadAPIReply(QVariantMap)));
}

void PatientDataUpload::GenericUploadSlot(qint64 done, qint64 total)
{
    ui->progressBar->setVisible(true);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue((int)((done * 100) / total));
}

void PatientDataUpload::handleUploadAPIReply(QVariantMap aResult)
{
    QLOG_INFO() << "[Upload Patient Data Dialog] Processing Service Upload Result" << aResult;
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        Utils::DisplayMessageBox("Upload Successful", "The Data has been successfully uploaded", QMessageBox::Information);
        this->close();
    }
    else
    {
        //this->show();
        QLOG_DEBUG() << "[Upload Patient Data Dialog] Error Recd.";
        Utils::DisplayMessageBox(aResult["error"].toString(), aResult["errorString"].toString(), QMessageBox::Critical);
    }
}
