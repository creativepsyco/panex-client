#ifndef ADDPATIENT_H
#define ADDPATIENT_H

#include <QWidget>
#include <QVariantMap>

namespace Ui {
class AddPatient;
}

class AddPatient : public QWidget
{
    Q_OBJECT
    
public:
    explicit AddPatient(QWidget *parent = 0);
    ~AddPatient();
    
private slots:
    void handleAddPatientApiResult(QVariantMap aResult);
    void on_btnAddPatient_clicked();

private:
    Ui::AddPatient *ui;
    bool validate();
};

#endif // ADDPATIENT_H
