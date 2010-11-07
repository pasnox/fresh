#ifndef PICONMANAGER_H
#define PICONMANAGER_H

/*!
	\file pIconManager.h
	\date 2008-11-01
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief A cache class for icons and pixmaps
*/

#include "core/FreshExport.h"

#include <QCache>
#include <QIcon>
#include <QPair>
#include <QMap>

/*!
	\brief A cache class for fileNames, icons and pixmaps.
	\details This class allow to cache and share QPixmap & QIcon.
	\details Icons and pixmaps can easily be loaded on demand by using coresponding members.
*/
namespace pIconManager
{
	/*!
		\details A typedef for icons caching
	*/
	typedef QCache<QString, QIcon> QIconCache;
	typedef QPair<QString, QString> FileNamePair;
	typedef QMap<FileNamePair, QString> FileNameCache;
	
	// return the filepath of the icon named fileName in prefix folder ( check is done recursively )
	FRESH_EXPORT QString filePath( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	// return the QPixmap of the pixmap named fileName in prefix folder ( check is done recursively )
	FRESH_EXPORT QPixmap pixmap( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	// return the QIcon of the icon named fileName in prefix folder ( check is done recursively )
	FRESH_EXPORT QIcon icon( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
};

#endif // PICONMANAGER_H
