#ifndef PATIENTVIEW_H
#define PATIENTVIEW_H

#include <QWidget>

namespace Ui {
class PatientView;
}

class PatientView : public QWidget
{
    Q_OBJECT
    
public:
    explicit PatientView(QWidget *parent = 0);
    ~PatientView();
    
private:
    Ui::PatientView *ui;
};

#endif // PATIENTVIEW_H
