#include "addpatient.h"
#include "ui_addpatient.h"
#include "global_include.h"
#include "qpanexapp.h"
#include "mainwindow.h"
#include "patientapi.h"
#include <QComboBox>
#include <QLineEdit>

AddPatient::AddPatient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddPatient)
{
    ui->setupUi(this);
    op_mode = NEW_MODE;
}

AddPatient::~AddPatient()
{
    delete ui;
}

bool AddPatient::validate()
{
    foreach(QLineEdit *widget, this->findChildren<QLineEdit*>()) {
        if (widget->text().length()==0)
            return false;
    }
    return true;
}

QVariantMap AddPatient::populateDataMap()
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
    return dataMap;
}

void AddPatient::on_btnAddPatient_clicked()
{
    // Set up the data to send
    if (this->validate() && this->op_mode == NEW_MODE)
    {
        QVariantMap dataMap = populateDataMap();
        connect(PanexApi::instance(), SIGNAL(AddPatientResultSignal(QVariantMap)),
                this, SLOT(handleAddPatientApiResult(QVariantMap)));

        PanexApi::instance()->AddPatient(dataMap);
    }
    else if (this->validate() && this->op_mode == EDIT_MODE)
    {
        QVariantMap dataMap = populateDataMap();
        dataMap.insert("id", this->patient_id);
        PatientAPI* pda = PanexApi::instance()->patientAPI();
        connect(pda, SIGNAL(EditPatientInfoResultSignal(QVariantMap)),
                this, SLOT(handleAddPatientApiResult(QVariantMap)));

        pda->EditPatient(dataMap);
    }
    else
    {
        Utils::DisplayMessageBox("Validation Error", "Please Fill out all the fields", QMessageBox::Critical);
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
        Utils::DisplayMessageBox("Success", "Successfully Carried out the operation", QMessageBox::Information);
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

void AddPatient::show()
{
    if (op_mode == NEW_MODE)
        QWidget::show();
    else if (op_mode == VIEW_MODE)
    {
        handleViewMode();
        QWidget::show();
    }
    else if (op_mode == EDIT_MODE)
    {
        handleEditMode();
        QWidget::show();
    }
}

void AddPatient::handleEditMode()
{
    setupPatientId();
    if (this->patient_id.compare("") == 0)
    {
        Utils::DisplayMessageBox("Error Encounterd", "Please Select a patient first.", QMessageBox::Critical);
        return;
    }
    PatientAPI* pda = PanexApi::instance()->patientAPI();
    connect(pda, SIGNAL(GetPatientInfoResultSignal(QVariantMap)), this, SLOT(handleGetPatientAPIReply(QVariantMap)));
    pda->GetPatientInfo(this->patient_id);
}

void AddPatient::disableFormControls()
{
    foreach(QWidget *widget, this->findChildren<QWidget*>()) {
        widget->setEnabled(false);
    }
    //    ui->txtAddress->setEnabled(false);
    //    ui->txtNotes->setEnabled(false);
    //    ui->btnAddPatient->setEnabled(false);
    //    ui->btnReset->setEnabled(false);
    //    ui->ethnicityChoice->setEnabled(false);
    //    ui->genderChoice->setEnabled(false);
}

void AddPatient::setupPatientId()
{
    this->patient_id= "";
    if (QPanexApp::instance()->mainWindow()->patientViewDialog())
        this->patient_id = QPanexApp::instance()->mainWindow()->patientViewDialog()->patient_id;
}

void AddPatient::handleViewMode()
{
    setupPatientId();

    if (this->patient_id.compare("") == 0)
    {
        Utils::DisplayMessageBox("Error Encounterd", "Please Select a patient first.", QMessageBox::Critical);
        return;
    }

    // Lock all the controls on the form
    disableFormControls();

    PatientAPI* pda = PanexApi::instance()->patientAPI();
    connect(pda, SIGNAL(GetPatientInfoResultSignal(QVariantMap)), this, SLOT(handleGetPatientAPIReply(QVariantMap)));
    pda->GetPatientInfo(this->patient_id);
}

/**
 * @brief AddPatient::handleGetPatientAPIReply
 *          Handles the API reply
 * @param aResult
 */
void AddPatient::populateForm(QVariantMap data)
{
    ui->txtAddress->setText(data["address"].toString());
    ui->txtPatientLastName->setText(data["lastName"].toString());
    ui->txtPatientFirstName->setText(data["firstName"].toString());
    ui->txtEmail->setText(data["email"].toString());
    ui->txtMobileNumber->setText(data["mobileNumber"].toString());
    ui->txtPhoneNumber->setText(data["phoneNumber"].toString());
    ui->txtIdNumber->setText(data["identificationNumber"].toString());
    ui->txtNotes->setText(data["notes"].toString());
    ui->ethnicityChoice->setCurrentIndex(ui->ethnicityChoice->findText(data["ethnicity"].toString().toUpper()));
    if (data["gender"].toString().compare("M") == 0)
        ui->genderChoice->setCurrentIndex(0);
    else
        ui->genderChoice->setCurrentIndex(1);
    ui->dateOfBirth->setDate(QDate::fromString(data["dateOfBirth"].toString(),"yyyy-MM-dd"));
}

void AddPatient::handleGetPatientAPIReply(QVariantMap aResult)
{
    QLOG_INFO() << "[Patient View Dialog] Processing View Patient Result" << aResult;
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        // @emit the correct signal
        QVariantMap data = aResult["data"].toMap();
        populateForm(data);
    }
    else
    {
        Utils::DisplayMessageBox(aResult["error"].toString(), aResult["errorString"].toString(), QMessageBox::Critical);
    }
}
