#include "patientview.h"
#include "ui_patientview.h"
#include "json.h"
#include "utils.h"
#include "global_include.h"

#include <QTreeView>
#include <QDesktopServices>
using namespace QtJson;

#define ALL_PATIENTS 0
#define ID_COLUMN_INDEX 0

PatientView::PatientView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientView)
{
    this->patient_id = "";
    // setup the model
    ui->setupUi(this);
    this->patientList = new QStandardItemModel;

    // For test
    QList<QStandardItem *> preparedRow =prepareRow("first", "second", "third");
    this->patientList->appendRow(preparedRow);
    QList<QStandardItem *> secondRow =prepareRow("111", "222", "333");
    this->patientList->appendRow(secondRow);
    // End of Test
    ui->treeViewPatients->setModel(this->patientList);

    ui->treeViewPatients->expandAll();

    // Get the list of patients
    this->GetPatientList();

    // Patient Data listing
    this->patientDataList = new QStandardItemModel;
    ui->treePatientData->setModel(this->patientDataList);

    // Set some connections

}

PatientView::~PatientView()
{
    delete ui;
}

void PatientView::GetPatientList()
{
    connect(PanexApi::instance(), SIGNAL(GetPatientListResultSignal(QVariantMap)),
            this, SLOT(HandleGetPatientListApiReplySlot(QVariantMap)));
    PanexApi::instance()->GetPatientList(ALL_PATIENTS);
}

void PatientView::setUpListHeaders()
{
    QStringList labels;
    labels << "Id" <<"Last Name" << "First Name" << "Identification" << "Email" << "Gender"
           << "Contact" << "Notes";
    patientList->setHorizontalHeaderLabels(labels);
}

void PatientView::HandleGetPatientListApiReplySlot(QVariantMap aResult)
{
    QLOG_DEBUG() << "[Patient View] HandleGetPatientListApiReplySlot " ;
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        // reset the list
        this->patientList->clear();
        setUpListHeaders();

        QVariantList list = aResult["patients"].toList();
        foreach(QVariant patient, list)
        {
            QVariantMap aPatient = patient.toMap();
            // Add Standard Item
            QList<QStandardItem *> preparedRow = prepareRow(aPatient["id"].toString(),
                                                            aPatient["lastName"].toString(),
                                                            aPatient["firstName"].toString(),
                                                            aPatient["identificationNumber"].toString(),
                                                            aPatient["email"].toString(),
                                                            aPatient["gender"].toString(),
                                                            aPatient["phoneNumber"].toString(),
                                                            aPatient["notes"].toString());
            this->patientList->appendRow(preparedRow);
        }
        // Hide the first column - No use to the user
        this->ui->treeViewPatients->hideColumn(ID_COLUMN_INDEX);

    }
    else
    {
        //this->show();
        QLOG_DEBUG() << "[PatientView] Error Recd.";
        Utils::DisplayMessageBox(aResult["errorString"].toString(), aResult["message"].toString() , QMessageBox::Information);

    }
}

QList<QStandardItem *> PatientView::prepareRow(const QString &first,
                                               const QString &second,
                                               const QString &third)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(first);
    rowItems << new QStandardItem(second);
    rowItems << new QStandardItem(third);
    return rowItems;
}

// Assuming we only wanna display these columns
// Have sys_id to differentiate the rails server id
QList<QStandardItem *> PatientView::prepareRow(const QString &sys_id,
                                               const QString &firstName,
                                               const QString &lastName,
                                               const QString &id,
                                               const QString &email,
                                               const QString &gender,
                                               const QString &ph,
                                               const QString &notes)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(sys_id);
    rowItems << new QStandardItem(firstName);
    rowItems << new QStandardItem(lastName);
    rowItems << new QStandardItem(id);
    rowItems << new QStandardItem(email);
    rowItems << new QStandardItem(gender);
    rowItems << new QStandardItem(ph);
    rowItems << new QStandardItem(notes);
    return rowItems;
}

///
/// \brief PatientView::on_treeViewPatients_activated
/// \param index
///
/// TODO: Correct handling of index needed
void PatientView::on_treeViewPatients_activated(const QModelIndex &index)
{
    // Start the process of loading from the patient data list
    bool ok;
    PatientDataAPI *apiObj = PanexApi::instance()->patientDataAPI();
    connect(apiObj, SIGNAL(GetPatientDataResultSignal(QVariantMap)),
            this, SLOT(HandleGetPatientDataApiReplySlot(QVariantMap)));
    apiObj->GetPatientDataList(this->patient_id.toInt(&ok, 10));
}


void PatientView::HandleGetPatientDataApiReplySlot(QVariantMap aResult)
{
//    QLOG_DEBUG() << "[PatientView] Data API Reply Recd. "<< aResult;
    QString success = "success";
    QString result  = aResult["result"].toString();
    if (result.compare(success) == 0)
    {
        this->patientDataList->clear();
        QStringList labels;
        labels << "Id" <<"File Name" << "Condition" << "Description" << "Data Type" << "Content Type";
        this->patientDataList->setHorizontalHeaderLabels(labels);

        QVariantList list = aResult["data"].toList();
        foreach(QVariant aData, list)
        {
            QVariantMap aDataObj = aData.toMap();
            // Add Standard Item
            QList<QStandardItem *> preparedRow;
            preparedRow << new QStandardItem(aDataObj["id"].toString());
            preparedRow << new QStandardItem(aDataObj["dataFile_file_name"].toString());
            preparedRow << new QStandardItem(aDataObj["condition"].toString());
            preparedRow << new QStandardItem(aDataObj["description"].toString());
            preparedRow << new QStandardItem(aDataObj["dataType"].toString());
            preparedRow << new QStandardItem(aDataObj["dataFile_content_type"].toString());

            this->patientDataList->appendRow(preparedRow);
        }
        // Hide the first column - No use to the user
        this->ui->treePatientData->hideColumn(ID_COLUMN_INDEX);
        this->ui->treePatientData->expandAll();

    }
    else
    {
        //this->show();
        QLOG_DEBUG() << "[PatientView] Error Recd.";
        Utils::DisplayMessageBox(aResult["errorString"].toString(), aResult["message"].toString() , QMessageBox::Information);
    }
}

///
/// \brief PatientView::on_treeViewPatients_clicked
/// \param index : Visual Tree Index passed by the calling function
///
void PatientView::on_treeViewPatients_clicked(const QModelIndex &index)
{
    this->patient_id = this->patientList->item(index.row(), ID_COLUMN_INDEX)->text();
//    QLOG_DEBUG() << "[PatientView on_treeViewPatients_clicked]" << this->patient_id << " has been selected";
}

#define DATATYPE_COLUMN_INDEX 4
void PatientView::on_treePatientData_activated(const QModelIndex &index)
{
    QString file_id = this->patientDataList->item(index.row(), ID_COLUMN_INDEX)->text();
    // further checks etc for Generic type
    QString file_type = this->patientDataList->item(index.row(), DATATYPE_COLUMN_INDEX)->text();
    PatientDataAPI *apiObj = PanexApi::instance()->patientDataAPI();
    connect(apiObj, SIGNAL(PatientDataDownloadFinished(QString,QString)), this, SLOT(FileDownloadFinishedSlot(QString,QString)));
    apiObj->DownloadPatientDataFile(file_id, this->patient_id, file_type);
}

void PatientView::FileDownloadFinishedSlot(QString path, QString error)
{
    QLOG_DEBUG() << "[Patient View] Downloaded File Path: " << path << "\n " << error;
    if (error.length() == 0)
    {
        // Open file
        QUrl fileUrl("file:///" + path);
        QDesktopServices::openUrl(fileUrl);
    }
    else
    {
        Utils::DisplayMessageBox(error, "Could not download the file, check network connection and try again in a few minutes", QMessageBox::Critical);
    }
}
