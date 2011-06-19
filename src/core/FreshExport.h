/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : FreshExport.h
** Date      : 2011-02-20T00:44:21
** License   : LGPL v3
** Home Page : http://bettercodes.org/projects/fresh
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
#ifndef FRESH_EXPORT_H
#define FRESH_EXPORT_H

/*!
    \file FreshExport.h
    \brief This macro allow symbols to be exported/imported for Windows OS.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include <qglobal.h>

/*!
    \def FRESH_EXPORT
    \brief This macro allow symbols to be exported/imported for Windows OS.
*/

#if defined( FRESH_CORE_BUILD )
#define FRESH_EXPORT Q_DECL_EXPORT
#else
#define FRESH_EXPORT Q_DECL_IMPORT
#endif

#endif // FRESH_EXPORT_H
