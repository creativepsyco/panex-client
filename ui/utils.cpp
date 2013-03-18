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

QString Utils::ConvertMapToString(QVariant aMap)
{
    QVariantMap map = aMap.toMap();
    QString finalStr = "";
    QList<QString> keys = map.keys();
    foreach (QString key, keys)
    {
        finalStr = finalStr + key + " ";
        // Need a check to see if it is string or qmap
        QVariant s = map[key];
        if (s.canConvert(QVariant::List))
        {
            QList<QVariant> list = s.toList();
            foreach (QVariant l, list)
            {
                finalStr = finalStr + l.toString() + "\n";
            }
        }
    }
    return finalStr;
}
