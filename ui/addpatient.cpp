#include "addpatient.h"
#include "ui_addpatient.h"
#include "global_include.h"
#include "qpanexapp.h"
#include "mainwindow.h"

AddPatient::AddPatient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddPatient)
{
    ui->setupUi(this);
}

AddPatient::~AddPatient()
{
    delete ui;
}

bool AddPatient::validate()
{
    // TODO: Validate the various controls
    return true;
}

void AddPatient::on_btnAddPatient_clicked()
{
    // Set up the data to send
    if (this->validate())
    {
        QVariantMap dataMap;
        dataMap.insert("lastName", ui->txtPatientLastName->text());
        dataMap.insert("firstName", ui->txtPatientFirstName->text());
        dataMap.insert("email", ui->txtEmail->text());
        dataMap.insert("address", ui->txtAddress->toPlainText());
        dataMap.insert("notes", ui->txtNotes->toPlainText());
        dataMap.insert("phoneNumber", ui->txtPhoneNumber->text());
        dataMap.insert("mobileNumber", ui->txtMobileNumber->text());
        dataMap.insert("identificationNumber", ui->txtIdNumber->text());
        dataMap.insert("ethnicity", ui->ethnicityChoice->currentText());
        dataMap.insert("gender", QString(ui->genderChoice->currentText()[0].toUpper()));
        dataMap.insert("dateOfBirth", ui->dateOfBirth->date().toString("yyyy-MM-dd"));

        connect(PanexApi::instance(), SIGNAL(AddPatientResultSignal(QVariantMap)),
                this, SLOT(handleAddPatientApiResult(QVariantMap)));

        PanexApi::instance()->AddPatient(dataMap);
    }
}


void AddPatient::handleAddPatientApiResult(QVariantMap aResult)
{
    QLOG_INFO() << "[Patient Add Dialog] Processing Add Patient Result" << aResult;
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        // emit the correct signal
        Utils::DisplayMessageBox("Success", "Successfully Added the Patient", QMessageBox::Information);
        QPanexApp::instance()->mainWindow()->patientViewDialog()->GetPatientList();
        this->on_btnReset_clicked();
    }
    else
    {
        //this->show();
        Utils::DisplayMessageBox(aResult["error"].toString(), aResult["errorString"].toString(), QMessageBox::Critical);
    }
}

void AddPatient::on_btnReset_clicked()
{
    foreach(QLineEdit *widget, this->findChildren<QLineEdit*>()) {
        widget->clear();
    }

    ui->txtAddress->clear();
    ui->txtNotes->clear();
}
