#ifndef PICONMANAGER_H
#define PICONMANAGER_H

/*!
	\file pIconManager.h
	\brief A cache class for icons and pixmaps
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"

#include <QCache>
#include <QIcon>
#include <QPair>
#include <QMap>

/*!
	\ingroup Gui
	\namespace pIconManager
	\brief A cache class for icons and pixmaps.
	
	This class allow to cache and share QPixmap & QIcon.
	Icons and pixmaps can easily be loaded on demand by using coresponding members.
*/
namespace pIconManager
{
	typedef QCache<QString, QIcon> QIconCache; // structure for QIcon cache.
	typedef QPair<QString, QString> FileNamePair; // structure for mapping user path to true path.
	typedef QMap<FileNamePair, QString> FileNameCache; // cache map for paths.
	
	/*!
		\brief Return the filepath of the file \a fileName in \a prefix folder (check is done recursively).
		\param fileName The file name of the file to find (ie: toto.txt).
		\param prefix The start path to look from, the scan is done recursively until a match or nothing found.
		\return A QString containing the absolute file path of the file, the fileName/prefix pair is cached for fast lookup the next call.
	*/
	FRESH_EXPORT QString filePath( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	/*!
		\brief Return the filepath of the pixmap \a fileName in \a prefix folder (check is done recursively).
		\param fileName The file name of the pixmap to find (ie: toto.png).
		\param prefix The start path to look from, the scan is done recursively until a match or nothing found.
		\return A QString containing the absolute file path of the pixmap, the fileName/prefix pair is cached for fast lookup the next call.
	*/
	FRESH_EXPORT QPixmap pixmap( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	/*!
		\brief Return the filepath of the icon \a fileName in \a prefix folder (check is done recursively).
		\param fileName The file name of the icon to find (ie: toto.png).
		\param prefix The start path to look from, the scan is done recursively until a match or nothing found.
		\return A QString containing the absolute file path of the icon, the fileName/prefix pair is cached for fast lookup the next call.
	*/
	FRESH_EXPORT QIcon icon( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
};

#endif // PICONMANAGER_H
