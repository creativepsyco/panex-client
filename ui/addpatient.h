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

    void on_btnReset_clicked();

private:
    Ui::AddPatient *ui;
    bool validate();
};

#endif // ADDPATIENT_H
