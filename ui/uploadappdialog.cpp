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

#include "uploadappdialog.h"
#include "ui_uploadappdialog.h"
#include <QFileDialog>
#include <QDesktopServices>
#include "global_include.h"
#include "qpanexapp.h"
#include "utils.h"

UploadAppDialog::UploadAppDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadAppDialog)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
}

UploadAppDialog::~UploadAppDialog()
{
    delete ui;
}

void UploadAppDialog::on_btnFileChoose_clicked()
{
    ui->lblFileName->setText(
                QFileDialog::getOpenFileName(this,"Open App File",
                                             QDesktopServices::storageLocation(QDesktopServices::PicturesLocation),
                                             "Image Files (*.png *.jpg *.gif *.bmp *.jpeg *.svg *.tiff)"));

}


void UploadAppDialog::on_btnCancel_clicked()
{
    this->close();
}

bool UploadAppDialog::validate()
{
    bool isValidated = true;

    return isValidated;
}

void UploadAppDialog::on_btnUpload_clicked()
{
    QVariantMap savedUserData = QPanexApp::instance()->settingsDialog()->getUserData();
    if (validate())
    {
        connect(PanexApi::instance(), SIGNAL(GenericSignal(QVariantMap)),
                this, SLOT(handleUploadAPIReply(QVariantMap)));
        PanexApi::instance()->UploadApp(ui->txtDesc->toPlainText(),
                                        ui->txtName->text(), ui->txtVersion->text(),
                                        ui->txtLink->text(),ui->lblFileName->text(),
                                        "", savedUserData);
        ui->progressBar->show();
        ui->progressBar->startTimer(10);
    }
}

void UploadAppDialog::handleUploadAPIReply(QVariantMap aResult)
{
    QLOG_INFO() << "[Upload App Dialog] Processing App Upload Result" << aResult;
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        Utils::DisplayMessageBox("Upload Successful", "The App has been successfully uploaded", QMessageBox::Information);
        this->close();
    }
    else
    {
        //this->show();
        QLOG_DEBUG() << "[Upload App Dialog] Error Recd.";
        Utils::DisplayMessageBox(aResult["error"].toString(), aResult["errorString"].toString(), QMessageBox::Critical);
    }
}
