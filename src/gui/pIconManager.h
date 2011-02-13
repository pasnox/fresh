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
		Return the file path of the given \a fileName starting looking recursively from \a prefix.
		\note The fileName/prefix pair is cached for fast lookup for next call.
	*/
	FRESH_EXPORT QString filePath( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	/*!
		Return the pixmap \a fileName starting looking recursively from \a prefix.
		\note The fileName/prefix pair is cached for fast lookup for next call.
	*/
	FRESH_EXPORT QPixmap pixmap( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	/*!
		Return the icon \a fileName starting looking recursively from \a prefix.
		\note The fileName/prefix pair is cached for fast lookup for next call.
	*/
	FRESH_EXPORT QIcon icon( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
};

#endif // PICONMANAGER_H
