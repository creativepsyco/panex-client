#ifndef UTILS_H
#define UTILS_H
#include <QMessageBox>
#include <QVariantMap>
#include <QVariant>

class Utils
{

public:
    static void DisplayMessageBox(const QString msg, const QString infoTxt);
    static void DisplayMessageBox(const QString msg, const QString infoTxt, QMessageBox::Icon icon);
    static QString ConvertMapToString(QVariant map);

};

#endif // UTILS_H
