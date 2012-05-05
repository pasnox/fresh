/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : Fresh.h
** Date      : 2011-02-20T00:41:35
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
#ifndef FRESH_H
#define FRESH_H

/*!
    \file Fresh.h
    \brief Access to library informations.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QString>

/*!
    \def FRESH_VERSION
    \brief This macro contains the Fresh version.
*/
#define FRESH_VERSION "1.1.0"

/*!
    \ingroup FreshCore
    \namespace Fresh
    \brief Access to library informations.
*/
namespace Fresh
{
    /*!
        Return the QString version of the library.
    */
    FRESH_EXPORT QString versionString();
    /*!
        Return the const char* version of the library.
    */
    FRESH_EXPORT const char* version();
};

#endif // FRESH_H
