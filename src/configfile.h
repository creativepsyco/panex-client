#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QSettings>

struct AppVersion
{
    uint majorVer;
    uint minorVer;
    uint patchVer;

    AppVersion( const QString &version );
    AppVersion( uint maj, uint min, uint pat );

    QString toString() const;
    void fromString( const QString &version );

    bool operator ==( const AppVersion &other ) const;
    bool operator !=( const AppVersion &other ) const;
    bool operator >( const AppVersion &other ) const;
    bool operator <( const AppVersion &other ) const;
    bool operator >=( const AppVersion &other ) const;
    bool operator <=( const AppVersion &other ) const;
};


class ConfigFile : public QSettings
{
public:
    ConfigFile();
    static const QString APP_VERSION;
    static const QString COMPAT_SETTINGS_APP_VERSION;

private:

};

#endif // CONFIGFILE_H
