#include <QMessageBox>
#include "utils.h"

const QString title = "Panex";

void Utils::DisplayMessageBox(const QString msg, const QString infoTxt, QMessageBox::Icon icon)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setIcon(icon);
    msgBox.setWindowTitle(title);
    msgBox.setInformativeText(infoTxt);
    msgBox.exec();
}

void Utils::DisplayMessageBox(const QString msg, const QString infoTxt)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setWindowTitle(title);
    msgBox.setInformativeText(infoTxt);
    msgBox.exec();
}
