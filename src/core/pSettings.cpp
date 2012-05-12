/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pSettings.cpp
** Date      : 2011-02-20T00:44:25
** License   : LGPL v3
** Home Page : https://github.com/pasnox/fresh
** Comment   : Fresh Library is a Qt 4 extension library providing set of new core & gui classes.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Leser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "pSettings.h"

#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>

#if defined( QT_GUI_LIB )
#include <QDesktopServices>
#include <QMainWindow>
#endif

pSettings::Properties pSettings::mDefaultProperties = pSettings::Properties( QString::null, QString::null, pSettings::Auto );

pSettings::Properties::Properties( const pSettings::Properties& other )
{
    operator=( other );
}

pSettings::Properties::Properties( const QString& _name, const QString& _version, pSettings::Type _type )
{
    name = _name;
    version = _version;
    type = _type;
}

QString pSettings::Properties::storageLocation() const
{
#if defined( QT_GUI_LIB )
    return QDesktopServices::storageLocation( QDesktopServices::DataLocation );
#else
    #warning May need a better way for major platforms
    return QDir::homePath().append( QString( ".%1" ).arg( name ) );
#endif
}

bool pSettings::Properties::isWritable( const QString& filePath ) const
{
    QFileInfo fi( filePath );
    
    while ( !fi.exists() ) {
        fi.setFile( fi.absolutePath() );
    }
    
    return fi.isWritable();
}

QString pSettings::Properties::filePath( pSettings::Type type ) const
{
    if ( name.isEmpty() || version.isEmpty() ) {
        Q_ASSERT( 0 );
        qFatal( "%s: name and version can not be empty", Q_FUNC_INFO );
    }
    
    QString fn;
    
    switch ( type ) {
        case pSettings::Invalid: {
            Q_ASSERT( 0 );
            qFatal( "%s: Invalid call", Q_FUNC_INFO );
            break;
        }
        case pSettings::Auto: {
#ifdef Q_OS_MAC
            const QString path = QString( "%1/../Resources" ).arg( QCoreApplication::applicationDirPath() );
#else
            const QString path = QCoreApplication::applicationDirPath();
#endif
            
            if ( isWritable( path ) ) {
                return filePath( pSettings::Portable );
            }
            
            // do nothing to throw pSettings::Normal case.
        }
        case pSettings::Normal: {
            fn = QString( "%1/%2-%3.ini" )
                .arg( storageLocation() )
                .arg( name )
                .arg( version );
            break;
        }
        case pSettings::Portable: {
#ifdef Q_OS_MAC
            fn = QString( "%1/../Resources/%2-%3.ini" )
                .arg( QCoreApplication::applicationDirPath() )
                .arg( name )
                .arg( version );
#else
            fn = QString( "%1/%2-%3.ini" )
                .arg( QCoreApplication::applicationDirPath() )
                .arg( name )
                .arg( version );
#endif
        }
    }
    
    if ( !isWritable( fn ) ) {
        if ( type == pSettings::Portable ) {
            //qWarning( "%s: File '%s' is not writable, fallbacking to pSettings::Normal mode.", Q_FUNC_INFO, fn.toLocal8Bit().constData() );
            return filePath( pSettings::Normal );
        }
        else {
            qWarning( "%s: File '%s' is not writable.", Q_FUNC_INFO, fn.toLocal8Bit().constData() );
            return QString::null;
        }
    }
    
    return QDir::cleanPath( fn );
}

QString pSettings::Properties::settingsFilePath() const
{
    return filePath( type );
}

QString pSettings::Properties::settingsFilePath( const QString& name, const QString& version ) const
{
    return QDir::cleanPath( QString( "%1/%2-%3.ini" )
        .arg( QFileInfo( settingsFilePath() ).absolutePath() )
        .arg( name )
        .arg( version ) );
}

pSettings::pSettings( QObject* parent, const pSettings::Properties& properties )
    : QSettings( properties.settingsFilePath(), QSettings::IniFormat, parent )
{
    mProperties = properties;
}

pSettings::pSettings( QObject* parent, const QString& name, const QString& version, const pSettings::Properties& properties )
    : QSettings( properties.settingsFilePath( name, version ), QSettings::IniFormat, parent )
{
    mProperties.name = name;
    mProperties.version = version;
    mProperties.type = properties.type;
}

pSettings::Properties pSettings::properties() const
{
    return mProperties;
}

#if defined( QT_GUI_LIB )
void pSettings::restoreState( QMainWindow* window )
{
    if ( window ) {
        //window->restoreGeometry( value( "MainWindow/Geometry" ).toByteArray() );
        window->setGeometry( value( "MainWindow/Geometry", window->geometry() ).toRect() );
        window->restoreState( value( "MainWindow/State" ).toByteArray() );
        
        //if ( value( "MainWindow/Geometry" ).toByteArray().isEmpty() ) {
        if ( value( "MainWindow/Geometry" ).toRect().isNull() ) {
            window->showMaximized();
        }
    }
}

void pSettings::saveState( QMainWindow* window )
{
    if ( window ) {
        //setValue( "MainWindow/Geometry", window->saveGeometry() );
        setValue( "MainWindow/Geometry", window->geometry() );
        setValue( "MainWindow/State", window->saveState() );
    }
}
#endif

void pSettings::setDefaultSettings()
{
}

void pSettings::setDefaultProperties( const pSettings::Properties& properties )
{
    mDefaultProperties = properties;
}

pSettings::Properties pSettings::defaultProperties()
{
    return mDefaultProperties;
}
