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

/*!
	\ingroup Core
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
};

#endif // PCOREUTILS_H
