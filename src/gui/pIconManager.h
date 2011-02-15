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
	\class pIconManager
	\brief A cache class for icons and pixmaps.
	
	This class allow to cache and share QPixmap & QIcon.
	Icons and pixmaps can easily be loaded on demand by using coresponding members.
*/
class FRESH_EXPORT pIconManager
{
private:
	typedef QCache<QString, QIcon> pIconCache; // structure for QIcon cache.
	typedef QPair<QString, QString> pFileNamePair; // structure for mapping user path to true path.
	typedef QMap<pFileNamePair, QString> pFileNameCache; // cache map for paths.
	
	static pIconManager::pIconCache mIconCache;
	static pIconManager::pFileNameCache mFileNameCache;
	
public:
	/*!
		Return the file path of the given \a fileName starting looking recursively from \a prefix.
		\note The fileName/prefix pair is cached for fast lookup for next call.
	*/
	static QString filePath( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	/*!
		Return the pixmap \a fileName starting looking recursively from \a prefix.
		\note The fileName/prefix pair is cached for fast lookup for next call.
	*/
	static QPixmap pixmap( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	/*!
		Return the icon \a fileName starting looking recursively from \a prefix.
		\note The fileName/prefix pair is cached for fast lookup for next call.
	*/
	static QIcon icon( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
};

#endif // PICONMANAGER_H
