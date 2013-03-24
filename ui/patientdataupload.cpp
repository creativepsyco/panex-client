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
#include <QFileDialog>
#include <QStringList>
#include <QTreeView>
#include "ui_patientdataupload.h"
#include "global_include.h"

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

    setHeaders();

    // For test
    QList<QStandardItem *> preparedRow =prepareRow("first", "second", "third");
    this->fileList->appendRow(preparedRow);
    QList<QStandardItem *> secondRow =prepareRow("111", "222", "333");
    this->fileList->appendRow(secondRow);
    // End of Test
    ui->treeFileList->setModel(this->fileList);
    ui->treeFileList->expandAll();

}

PatientDataUpload::~PatientDataUpload()
{
    delete ui;
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


