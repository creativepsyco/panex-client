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

#include "uploadservicedialog.h"
#include "ui_uploadservicedialog.h"
#include <QDesktopServices>
#include <QFileDialog>
#include "qpanexapp.h"
#include "global_include.h"
#include <QVariantMap>
#include "utils.h"

UploadServiceDialog::UploadServiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadServiceDialog)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
}

UploadServiceDialog::~UploadServiceDialog()
{
    delete ui;
}

void UploadServiceDialog::on_btnThumbnailChoose_clicked()
{
    ui->lblThumbFileName->setText(
                QFileDialog::getOpenFileName(this,"Select Service Thumbnail",
                                             QDesktopServices::storageLocation(QDesktopServices::PicturesLocation),
                                             "Image Files (*.png *.jpg *.gif *.bmp *.jpeg *.svg *.tiff)"));
}

void UploadServiceDialog::on_btnServiceFileChoose_clicked()
{
    ui->lblServiceFileName->setText(
                QFileDialog::getOpenFileName(this,"Open App File",
                                             QDesktopServices::storageLocation(QDesktopServices::HomeLocation),
                                             "Zip Files (*.zip *.rar)"));
}

void UploadServiceDialog::on_btnUpload_clicked()
{
    QVariantMap savedUserData = QPanexApp::instance()->settingsDialog()->getUserData();
    if (isValidated())
    {
        connect(PanexApi::instance(), SIGNAL(GenericSignal(QVariantMap)),
                this, SLOT(handleUploadAPIReply(QVariantMap)));
        PanexApi::instance()->UploadService(ui->txtDesc->toPlainText(),
                                        ui->txtName->text(), ui->txtVersion->text(),
                                        ui->txtLink->text(),ui->lblThumbFileName->text(),
                                        ui->lblServiceFileName->text(), ui->txtCmdLine->text(), savedUserData);
        connect(PanexApi::instance(), SIGNAL(GenericUploadProgressSignal(qint64,qint64)),
                this, SLOT(updateDataTransferProgress(qint64,qint64)));
        ui->progressBar->show();
    }
}

bool UploadServiceDialog::isValidated()
{
    return true;
}

void UploadServiceDialog::updateDataTransferProgress(qint64 readBytes,
                                                 qint64 totalBytes)
{
//    QLOG_DEBUG() << "[Called] " << readBytes << " \t" << totalBytes;
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue((int)((readBytes * 100) / totalBytes));
}

void UploadServiceDialog::handleUploadAPIReply(QVariantMap aResult)
{
    QLOG_INFO() << "[Upload Service Dialog] Processing Service Upload Result" << aResult;
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        Utils::DisplayMessageBox("Upload Successful", "The Service has been successfully uploaded", QMessageBox::Information);
        this->close();
    }
    else
    {
        //this->show();
        QLOG_DEBUG() << "[Upload Service Dialog] Error Recd.";
        Utils::DisplayMessageBox(aResult["error"].toString(), aResult["errorString"].toString(), QMessageBox::Critical);
    }
}

void UploadServiceDialog::on_btnCancel_clicked()
{
    this->reject();
}
