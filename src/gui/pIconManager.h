/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pIconManager.h
** Date      : 2011-02-20T00:41:35
** License   : LGPL v3
** Home Page : https://github.com/pasnox/fresh
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
#ifndef PICONMANAGER_H
#define PICONMANAGER_H

/*!
	\file pIconManager.h
	\brief A cache class for icons and pixmaps
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QCache>
#include <QIcon>
#include <QPair>
#include <QMap>

/*!
	\ingroup FreshGui
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
