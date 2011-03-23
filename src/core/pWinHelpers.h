/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pMacHelpers.h
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
#ifndef PWINHELPERS_H
#define PWINHELPERS_H

/*!
	\file pWinHelpers.h
	\brief A central place for windows helpers.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include <QtGlobal>

/*!
	\ingroup FreshCore
	\namespace pWinHelpers
	\brief A central place for windows helpers.
	\note This namespace/macros are available on Windows only.
*/
#ifdef UNICODE /*UNICODE*/
#ifndef _UNICODE // needed for _tcslen
#define _UNICODE
#endif
#define QStringToTCHAR(x)     (wchar_t*) x.utf16()
#define PQStringToTCHAR(x)    (wchar_t*) x->utf16()
#define TCHARToQString(x)     QString::fromUtf16((ushort*)(x))
#define TCHARToQStringN(x,y)  QString::fromUtf16((ushort*)(x),(y))
#else /*UNICODE*/
#define QStringToTCHAR(x)     x.local8Bit().constData()
#define PQStringToTCHAR(x)    x->local8Bit().constData()
#define TCHARToQString(x)     QString::fromLocal8Bit((x))
#define TCHARToQStringN(x,y)  QString::fromLocal8Bit((x),(y))
#endif /*UNICODE*/

// in case of need
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#endif // PWINHELPERS_H
