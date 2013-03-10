#include <QMessageBox>

const QString title = "Panex";

void DisplayMessageBox(const QString msg, const QString infoTxt, QMessageBox::Icon icon)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setIcon(icon);
    msgBox.setWindowTitle(title);
    msgBox.setInformativeText(infoTxt);
    msgBox.exec();
}
