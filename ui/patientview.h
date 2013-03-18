#ifndef PATIENTVIEW_H
#define PATIENTVIEW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include "global_include.h"
#include "panexapi.h"

namespace Ui {
class PatientView;
}

class PatientView : public QWidget
{
    Q_OBJECT
    
public:
    explicit PatientView(QWidget *parent = 0);
    void GetPatientList();
    ~PatientView();

private slots:
    void HandleGetPatientListApiReplySlot(QVariantMap dataMap);


private:
    QList<QStandardItem *> prepareRow(const QString &first,
                                      const QString &second,
                                      const QString &third);

    QList<QStandardItem *> prepareRow(const QString &first,
                                      const QString &second,
                                      const QString &third,
                                      const QString &fourth,
                                      const QString &fifth,
                                      const QString &sixth,
                                      const QString &seventh);
    Ui::PatientView *ui;
    QStandardItemModel *patientList;
};

#endif // PATIENTVIEW_H
