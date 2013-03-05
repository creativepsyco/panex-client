#include "addpatient.h"
#include "ui_addpatient.h"

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
