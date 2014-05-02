/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pEnvironmentVariablesManager.h
** Date      : 2011-02-20T00:41:09
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
#ifndef PENVIRONMENTVARIABLESMANAGER_H
#define PENVIRONMENTVARIABLESMANAGER_H

/*!
    \file pEnvironmentVariablesManager.h
    \brief Helper manager for pEnvironmentVariablesModel.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pEnvironmentVariablesModel.h"

/*!
    \ingroup FreshGui
    \class pEnvironmentVariablesManager
    \brief Helper manager for pEnvironmentVariablesModel.

    This helper class allow to read, write, update and merge variables to/from files.
*/
class FRESH_EXPORT pEnvironmentVariablesManager
{
public:
    /*!
        Create a manager for handling pEnvironmentVariablesModel::Variables.
    */
    pEnvironmentVariablesManager();
    /*
        Destroys the manager.
    */
    virtual ~pEnvironmentVariablesManager();
    /*!
        Load the variables states from ini file.
    */
    bool load();
    /*!
        Save the variables states to ini file.
    */
    bool save();
    /*!
        Return the variables of this manager.
    */
    pEnvironmentVariablesModel::Variables variables() const;
    /*!
        Set the variables of this manager.
    */
    void setVariables( const pEnvironmentVariablesModel::Variables& variables );
    /*!
        Check for new variables defined in QProcessEnvironment::systemEnvironment() and merge them in \a variables.
        
        Returns true if changes occurs else false.
    */
    bool mergeNewVariables( pEnvironmentVariablesModel::Variables& variables ) const;
    /*!
        Check for new variables defined in QProcessEnvironment::systemEnvironment() and merge them with \a variables.
        
        Returns the merged version of \a variables.
    */
    pEnvironmentVariablesModel::Variables mergeNewVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
    /*!
        Remove from \a variables all unmodified variables.
        
        Unmododified variables are the ones that are enabled and content is same as defined in QProcessEnvironment::systemEnvironment().
        
        Returns true if changes occurs else false.
    */
    bool removeUnmodifiedVariables( pEnvironmentVariablesModel::Variables& variables ) const;
    /*!
        Remove from \a variables all unmodified variables.
        
        Unmododified variables are the ones that are enabled and content is same as defined in QProcessEnvironment::systemEnvironment().
        
        Returns the merged version of \a variables.
    */
    pEnvironmentVariablesModel::Variables removeUnmodifiedVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
    /*!
        Return a list of pairs of key=value variables.
        
        The disabled one are kept according to \a keepDisabled.
    */
    QStringList variables( bool keepDisabled ) const;

protected:
    static const QString mSettingsKey;
    bool mInitialized;
    mutable pEnvironmentVariablesModel::Variables mVariables;
    
    /*!
        This is the low level function that write \a variables to an ini file.
        
        The defauld implementation use pSettings for that.
    */
    virtual bool writeVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
    /*!
        This is the low level function that read \a variables from an ini file.
        
        The defauld implementation use pSettings for that.
    */
    virtual bool readVariables( pEnvironmentVariablesModel::Variables& variables ) const;
};

#endif // PENVIRONMENTVARIABLESMANAGER_H
