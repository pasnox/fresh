#ifndef PFILESYSTEMUTILS_H
#define PFILESYSTEMUTILS_H

/*!
	\file pFileSystemUtils.h
	\brief Global place for IO functions.
	
	This namespace is a global place for functions related to IO.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"

#include <QDir>

class QLocale;

/*!
	\namespace pFileSystemUtils
	\brief Global place for IO functions.
	
	This namespace is a global place for functions related to IO.
*/
namespace pFileSystemUtils
{
	/*!
		\brief Look for \a fileName in \a dir, the search is done recursively according to \a recursive.
		\param dir The start search directory.
		\param fileName The file name, this can be a mask file name like: README*.txt.
		\param recursive Tells if the scan of directories will recurse or not. The default is true.
		\return The absolute file path of the found file or QString::null.
	*/
	FRESH_EXPORT QString findFile( QDir& dir, const QString& fileName, bool recursive = true );
	/*!
		\brief Return the available list of text codecs.
		\return The codecs list.
		\note The list is ascending sorted by codec name;
	*/
	FRESH_EXPORT QStringList textCodecs();
	/*!
		\brief Implementation of QLocale hash.
		\param locale The locale to hash.
		\return An uint hash of the locale name;
		\sa qHash, QHash
	*/
	FRESH_EXPORT uint qHash( const QLocale& locale );
#if QT_VERSION < 0x040700
	/*!
		\brief Implementation of QUrl hash.
		\param url The url to hash.
		\return An uint hash of the url.toString();
		\sa qHash, QHash
		\note This member is defined only if QT_VERSION < 4.7.0 because it exists for Qt version >= 4.7.0.
	*/
	FRESH_EXPORT uint qHash( const QUrl& url );
#endif
};

#endif // PFILESYSTEMUTILS_H
