#include "configfile.h"

#include <QFileInfo>
#include <QStringList>

AppVersion::AppVersion( uint maj, uint min, uint pat ) :
    majorVer( maj ),
    minorVer( min ),
    patchVer( pat )
{
}

AppVersion::AppVersion( const QString &version )
{
    fromString( version );
}

QString AppVersion::toString() const
{
    return QString( "%1.%2.%3" ).arg( QString::number(majorVer),
                                      QString::number(minorVer),
                                      QString::number(patchVer) );
}

void AppVersion::fromString( const QString &version )
{
    QStringList parts = version.split( "." );
    if ( parts.size() == 3 ) {
        majorVer = parts.at(0).toUInt();
        minorVer = parts.at(1).toUInt();
        patchVer = parts.at(2).toUInt();
    }
}

bool AppVersion::operator ==( const AppVersion &other ) const
{
    return majorVer == other.majorVer && minorVer == other.minorVer && patchVer == other.patchVer;
}

bool AppVersion::operator !=( const AppVersion &other ) const
{
    return !( *this == other );
}

bool AppVersion::operator >( const AppVersion &other ) const
{
    if ( majorVer != other.majorVer ) {
        return majorVer > other.majorVer;
    } else if ( minorVer != other.minorVer ) {
        return minorVer > other.minorVer;
    } else {
        return patchVer > other.patchVer;
    }
}

bool AppVersion::operator <( const AppVersion &other ) const
{
    if ( *this != other ) {
        return !( *this > other );
    } else {
        return false;
    }
}

bool AppVersion::operator >=( const AppVersion &other ) const
{
    return ( *this == other ) || ( *this > other );
}

bool AppVersion::operator <=( const AppVersion &other ) const
{
    return ( *this == other ) || ( *this < other );
}

const QString ConfigFile::APP_VERSION = "0.10.0";
const QString ConfigFile::COMPAT_SETTINGS_APP_VERSION = "0.9.0";


ConfigFile settings;


ConfigFile::ConfigFile():
    #if defined Q_WS_MAC
    QSettings( QSettings::defaultFormat(), QSettings::UserScope, "panex.sg", "qPanex" )
  #elif defined Q_WS_WIN
    QSettings( QSettings::IniFormat, QSettings::UserScope, "panex", "qPanex" )
  #else
    QSettings( QSettings::defaultFormat(), QSettings::UserScope, "panex", "qPanex" )
  #endif
{
    if ( QFileInfo( fileName() ).exists() ) {
        if ( contains( "FIRSTRUN" ) ) {
            remove( "FIRSTRUN" );
        }

        QString ver = value( "General/version", QString() ).toString();
    } else {
        setValue( "FIRSTRUN", "ever" );
    }
    setValue( "General/version", ConfigFile::APP_VERSION );
    sync();
}

