#ifndef UTILS_H
#define UTILS_H
#include <QMessageBox>

class Utils
{

public:
    static void DisplayMessageBox(const QString msg, const QString infoTxt);
    static void DisplayMessageBox(const QString msg, const QString infoTxt, QMessageBox::Icon icon);

};

#endif // UTILS_H
