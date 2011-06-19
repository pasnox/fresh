/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pCoreUtils.h
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
#ifndef PCOREUTILS_H
#define PCOREUTILS_H

/*!
	\file pCoreUtils.h
	\brief A central place for reusable core functions.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QDir>

class QLocale;
class QUrl;

/*!
	Return a hash for \a locale.
	\sa qHash, QHash.
*/
FRESH_EXPORT uint qHash( const QLocale& locale );

#if QT_VERSION < 0x040700
/*!
	Return a hash for \a url, in fact it return qHash( url.toString() ).
	\sa qHash, QHash.
	\note This member is defined only if QT_VERSION < 4.7.0 because it exists for Qt version >= 4.7.0.
*/
FRESH_EXPORT uint qHash( const QUrl& url );
#endif

/*!
	\ingroup FreshCore
	\namespace pCoreUtils
	\brief A central place for reusable core functions.
*/
namespace pCoreUtils
{
	/*!
		Return a file path that match \a fileName in \a dir.
		If no match is found QString::null is returned.
		The search is done recursively according to \a recursive.
		\note fileName can be a filter (ie: README*.TXT).
	*/
	FRESH_EXPORT QString findFile( QDir& dir, const QString& fileName, bool recursive = true );
	/*!
		Return a list of file path that match \a filters in \a dir.
		If no match is found QStringList() is returned.
		The search is done recursively according to \a recursive.
	*/
	FRESH_EXPORT QStringList findFiles( QDir& dir, const QStringList& filters, bool recursive = true );
	/*!
		Return a list of file path that match \a filter in \a dir.
		If no match is found QStringList() is returned.
		The search is done recursively according to \a recursive.
	*/
	FRESH_EXPORT QStringList findFiles( QDir& dir, const QString& filter, bool recursive = true );
	/*!
		Return true if the directory at \a path is totally empty else false.
	*/
	FRESH_EXPORT bool isEmptyDirectory( const QString& path );
	/*!
		Return the available list of sorted text codecs.
	*/
	FRESH_EXPORT QStringList textCodecs();
	/*!
		Return a title cased string of \a string.
		
		Title case is the meaning of having each first letter of a word upercase and others lower case.
	*/
	FRESH_EXPORT QString toTitleCase( const QString& string );
	/*!
		Adapt file size \a nb to a user visible string.
	*/
	FRESH_EXPORT QString fileSizeAdaptString( qreal nb );
	/*!
		Convert \a bytes to a human reading string.
	*/
	FRESH_EXPORT QString fileSizeToString( qint64 bytes );
};

#endif // PCOREUTILS_H
