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

#include "servicerundialog.h"
#include "ui_servicerundialog.h"
#include "global_include.h"
#include "utils.h"
#include "qpanexapp.h"
#include "mainwindow.h"
#include "serviceapi.h"
#include <QTreeView>

#define ID_COLUMN_INDEX 0

ServiceRunDialog::ServiceRunDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServiceRunDialog)
{
    ui->setupUi(this);

    this->serviceListModel = new QStandardItemModel;

    ui->treeServiceList->setModel(this->serviceListModel);

    ServiceAPI *sAPI = PanexApi::instance()->serviceAPI();
    connect(sAPI, SIGNAL(ServiceListSignal(QVariantMap)), this, SLOT(handleServiceListReply(QVariantMap)));
    sAPI->getServiceList();
}

ServiceRunDialog::~ServiceRunDialog()
{
    delete ui;
}

void ServiceRunDialog::on_btnCancel_clicked()
{
    this->reject();
}

void ServiceRunDialog::setInputList(QVariantList theList)
{
    QLOG_DEBUG() << theList;
    this->inputFiles = theList;
}

void ServiceRunDialog::on_btnStart_clicked()
{
    if (QPanexApp::instance()->mainWindow()->patientViewDialog())
        setInputList(QPanexApp::instance()->mainWindow()->patientViewDialog()->GetSelectedPatientData());
    if(this->inputFiles.length() == 0)
    {
        Utils::DisplayMessageBox("Error", "Please Choose the patient data before continuing", QMessageBox::Critical);
        return;
    }

    this->patient_id = QPanexApp::instance()->mainWindow()->patientViewDialog()->patient_id;
    if (this->patient_id.length() == 0)
    {
        Utils::DisplayMessageBox("Error", "Patient is not Set please goto the data view before running the service", QMessageBox::Critical);
        return;
    }

    QModelIndexList selected_services = ui->treeServiceList->selectionModel()->selectedRows();
    QModelIndex selected_service = selected_services.at(0);// first one
    QString selected_service_id = this->serviceListModel->item(selected_service.row(), ID_COLUMN_INDEX)->text();

    // Get creator_id
    QVariantMap user_data = QPanexApp::instance()->settingsDialog()->getUserData();
    ServiceAPI *sAPI = PanexApi::instance()->serviceAPI();
    connect(sAPI, SIGNAL(GenericSignal(QVariantMap)), this, SLOT(handleGenericAPIReply(QVariantMap)));
    sAPI->startService(selected_service_id, this->patient_id, user_data["user_id"].toString(), this->inputFiles);
}

void ServiceRunDialog::setUpListHeaders()
{
    QStringList labels;
    labels << "Id" <<"Service Name" << "Service Description" << "Help Link" << "Version";
    this->serviceListModel->setHorizontalHeaderLabels(labels);
}

void ServiceRunDialog::handleServiceListReply(QVariantMap aResult)
{
    QLOG_DEBUG() << "[Service Run Dialog] handleServiceListReply" ;
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        // reset the list
        this->serviceListModel->clear();
        setUpListHeaders();

        QVariantList list = aResult["services"].toList();
        foreach(QVariant service, list)
        {
            QVariantMap aService = service.toMap();
            // Add Standard Item
            QList<QStandardItem *> preparedRow;
            preparedRow << new QStandardItem(aService["id"].toString());
            preparedRow << new QStandardItem(aService["name"].toString());
            preparedRow << new QStandardItem(aService["description"].toString());
            preparedRow << new QStandardItem(aService["helpLink"].toString());
            preparedRow << new QStandardItem(aService["version"].toString());

            this->serviceListModel->appendRow(preparedRow);
        }
        // Hide the first column - No use to the user
        this->ui->treeServiceList->hideColumn(ID_COLUMN_INDEX);

    }
    else
    {
        //this->show();
        QLOG_DEBUG() << "[ServiceRunDialog] Error Recd.";
        Utils::DisplayMessageBox(aResult["errorString"].toString(), aResult["message"].toString() , QMessageBox::Information);

    }
}

void ServiceRunDialog::handleGenericAPIReply(QVariantMap aResult)
{
    QLOG_DEBUG() << "[Service Run Dialog] handleServiceListReply" ;
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        QLOG_DEBUG() << "[ServiceRunDialog] Succesful";
        Utils::DisplayMessageBox("Success", "The operation was carried out successfully", QMessageBox::Information);
        this->close();
    }
    else
    {
        //this->show();
        QLOG_DEBUG() << "[ServiceRunDialog] Error Recd.";
        Utils::DisplayMessageBox(aResult["errorString"].toString(), aResult["message"].toString() , QMessageBox::Information);

    }
}
