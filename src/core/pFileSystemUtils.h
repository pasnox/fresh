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

/*!
	\namespace pFileSystemUtils
	\brief Global place for IO functions.
	
	This namespace is a global place for functions related to IO.
*/
namespace pFileSystemUtils
{
	/*!
		\brief Look for \a fileName in \a dir, the search is done recursively according to \a recursive.
		\param dir The start search directory
		\param fileName The file name, this can be a mask file name like: README*.txt
		\param recursive Tells if the scan of directories will recurse or not. The default is true
		\return The absolute file path of the found file or QString::null
	*/
	FRESH_EXPORT QString findFile( QDir& dir, const QString& fileName, bool recursive = true );
	/*!
		\brief Return the available list of text codecs.
		\return The codecs list
		\note The list is ascending sorted by codec name
	*/
	FRESH_EXPORT QStringList textCodecs();
};

#endif // PFILESYSTEMUTILS_H
