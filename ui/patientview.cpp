#include "patientview.h"
#include "ui_patientview.h"
#include "json.h"
#include "utils.h"
#include "global_include.h"

#include <QTreeView>
using namespace QtJson;

#define ALL_PATIENTS 0
#define ID_COLUMN_INDEX 0

PatientView::PatientView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientView)
{
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
    QModelIndexList indices = ui->treeViewPatients->selectionModel()->selectedRows();
//    QLOG_INFO() << indices.length() << " no. of indices were selected";
    foreach(QModelIndex index, indices)
    {
        // Add to the data view
        QStandardItemModel *model = new QStandardItemModel;
        QList<QStandardItem *> preparedRow =prepareRow("FileName", "DICOM", "Patient Data");
        model->appendRow(preparedRow);
        QList<QStandardItem *> secondRow =prepareRow("Another File", "PDF", "Service Result");
        model->appendRow(secondRow);
        QList<QStandardItem *> thirdRow =prepareRow("Result.txt", "TXT", "Service Result");
        model->appendRow(thirdRow);
        ui->treePatientData->setModel(model);
        ui->treePatientData->expandAll();
    }
}

void PatientView::on_treeViewPatients_clicked(const QModelIndex &index)
{
    this->patient_id = this->patientList->item(index.row(), ID_COLUMN_INDEX)->text();
    QLOG_DEBUG() << this->patient_id << " has been selected";
}
