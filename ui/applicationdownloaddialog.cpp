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

#include "applicationdownloaddialog.h"
#include "ui_applicationdownloaddialog.h"
#include <QFileDialog>
#include "global_include.h"
#include "panexapi.h"
#include <QTreeView>
#include <QDesktopServices>

ApplicationDownloadDialog::ApplicationDownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplicationDownloadDialog)
{
    ui->setupUi(this);
    appListModel = new QStandardItemModel;

    //  connect and fire
    connect(PanexApi::instance(), SIGNAL(GenericSignal(QVariantMap)), this, SLOT(HandleAppListAPIReply(QVariantMap)));
    PanexApi::instance()->GetAppList(0);
}

ApplicationDownloadDialog::~ApplicationDownloadDialog()
{
    delete ui;
}

void ApplicationDownloadDialog::on_pushButton_clicked()
{
    QModelIndexList selectedRow = ui->treeView->selectionModel()->selectedRows();
    QString id = appListModel->item(selectedRow[0].row(), 2)->text();
    QString url = (PanexApi::instance()->GetRootURL() + "/apps/%1.zip").arg(id);
    QUrl theUrl(url);
    QDesktopServices::openUrl(theUrl);

//    QFileDialog dialog(this);
//    dialog.setOption(QFileDialog::ShowDirsOnly, true);
//    dialog.setDirectory(QDir::homePath());
//    dialog.setFileMode(QFileDialog::DirectoryOnly);
//    // Set Filter ?
//    if (dialog.exec())
//    {
//        QLOG_DEBUG() << dialog.selectedFiles().count() << dialog.selectedFilter();
//    }

//    if (dialog.selectedFiles().count() == 1)
//    {
//        QString msg = "Selected %1 for download app";
//        updateStatusBar(msg.arg(dialog.selectedFiles()[0]));
//    }
}

void ApplicationDownloadDialog::on_pushButton_2_clicked()
{
    this->reject();
}

void ApplicationDownloadDialog::updateStatusBar(QString msg)
{
    QString message = "Message: " + msg;
    ui->statusText->setText(message);
}

void ApplicationDownloadDialog::setupHeaders()
{
    QStringList labels;
    labels << "Name" << "Description" << "id";
    appListModel->setHorizontalHeaderLabels(labels);
}

void ApplicationDownloadDialog::setupTreeView(QVariantList appList)
{
    appListModel->clear();
    foreach(QVariant app, appList)
    {
        QVariantMap appMap = app.toMap();
        QList<QStandardItem *> rowItems;
        rowItems << new QStandardItem(appMap["name"].toString());
        rowItems << new QStandardItem(appMap["description"].toString());
        rowItems << new QStandardItem(appMap["id"].toString());
        appListModel->appendRow(rowItems);
    }
    setupHeaders();
    ui->treeView->setModel(appListModel);
}

void ApplicationDownloadDialog::HandleAppListAPIReply(QVariantMap aReply)
{
    QString result = aReply["result"].toString();
    if(result.compare("success") == 0)
    {
        updateStatusBar("Successfully loaded Application list");
        QVariantList appList = aReply["apps"].toList();
        setupTreeView(appList);
    }
    else
    {
        Utils::DisplayMessageBox(aReply["errorString"].toString(), aReply["message"].toString() , QMessageBox::Information);
    }
}
