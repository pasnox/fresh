/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pSettings.h
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
#ifndef PSETTINGS_H
#define PSETTINGS_H

/*!
    \file pSettings.h
    \brief An extended QSettings class that handle ini files with different contexts.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QSettings>

#if defined( QT_GUI_LIB )
class QMainWindow;
#endif

/*!
    \ingroup FreshCore
    \class pSettings
    \brief An extended QSettings class that handle ini files with different contexts.
    
    This class is used for storing/retreiving data using ini files with different contexts (Auto, Normal, Portable).
*/
class FRESH_EXPORT pSettings : public QSettings
{
    Q_OBJECT

public:
    /*! Thie enumeration represent the possible choices for the location of your pSettings object. */
    enum Type {
        Invalid = -1, /*!< An invalid path. */
        Auto, /*!< The pSettings decide which enum to use. pSettings::Portable if the binary path is writable, else pSettings::Normal. */
        Normal, /*!< The init file will be created in the user home. */
        Portable /*!< The ini file will be created in the binary path if possible, else fallback to pSettings::Normal mode. */
    };
    
    /*!
        \struct pSettings::Properties
        \brief This class defined the properties to apply to a pSettings class.
        \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
    */
    struct FRESH_EXPORT Properties
    {
        /*!
            Create a properties object initialized with \a other or default properties.
        */
        Properties( const pSettings::Properties& other = pSettings::defaultProperties() );
        /*!
            Create a properties object initialized \a name, \a version and \a type.
        */
        Properties( const QString& name, const QString& version, pSettings::Type type );
        /*!
            Return the path where is located the ini file when using the pSettings::Normal type.
        */
        QString storageLocation() const;
        /*!
            Return a deducted file path for \a type.
        */
        QString filePath( pSettings::Type type ) const;
        /*!
            Return the absolute file path of the ini file.
        */
        QString settingsFilePath() const;
        /*!
            Return an absolute file path of a ini file having application name \a name and application version \a version according to the current properties.
        */
        QString settingsFilePath( const QString& name, const QString& version ) const;
        /*!
            Return true if \a filePath is writable (ie: file/path can be created/writed), else false.
        */
        bool isWritable( const QString& filePath ) const;
        
        QString name;
        QString version;
        pSettings::Type type;
    };
    /*!
        Create a pSettings object having parent \a parent and properties \a properties.
    */
    pSettings( QObject* parent = 0, const pSettings::Properties& properties = defaultProperties() );
    /*!
        Create a pSettings object having parent \a parent and properties \a properties with overrided \a name and \a version.
    */
    pSettings( QObject* parent, const QString& name, const QString& version, const pSettings::Properties& properties = defaultProperties() );
    /*!
        Return the properties applied to this pSettings.
    */
    pSettings::Properties properties() const;

#if defined( QT_GUI_LIB )
    /*!
        Restore the state of \a window.
        \sa QMainWindow::restoreState().
        \note This member is only defined if QT_GUI_LIB is defined.
    */
    virtual void restoreState( QMainWindow* window );
    /*!
        Save the state of \a window.
        \sa QMainWindow::saveState().
        \note This member is only defined if QT_GUI_LIB is defined.
    */
    virtual void saveState( QMainWindow* window );
#endif
    /*!
        Set the default settings.
        You can reimplement this member to set default ini settings, by example after the first application run.
    */
    virtual void setDefaultSettings();
    /*!
        Set the default properties to be used to \a properties.
        \note Typically this should be defined in the main, just after the initialization of the QCoreApplication / QApplication.
    */
    static void setDefaultProperties( const pSettings::Properties& properties );
    /*!
        Return the default Properties used when creating pSettings.
    */
    static pSettings::Properties defaultProperties();

protected:
    static pSettings::Properties mDefaultProperties;
    pSettings::Properties mProperties;
};

#endif // PSETTINGS_H
