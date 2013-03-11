#ifndef PANEXAPI_H
#define PANEXAPI_H

#include <QObject>
#include "global_include.h"

class PanexApi : public QObject
{
    Q_OBJECT
public:
    explicit PanexApi(QObject *parent = 0);
    
signals:
    
public slots:
private:
    static const QString *networkUrl;
    static const bool isDebug;
};

#endif // PANEXAPI_H
