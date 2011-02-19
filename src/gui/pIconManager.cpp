/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pIconManager.cpp
** Date      : 2011-02-20T00:41:35
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
#include "pIconManager.h"
#include "pCoreUtils.h"

#include <QPixmapCache>
#include <QDir>
#include <QDebug>

pIconManager::pIconCache pIconManager::mIconCache( 200 );
pIconManager::pFileNameCache pIconManager::mFileNameCache;

QString pIconManager::filePath( const QString& fileName, const QString& prefix )
{
	QString path = prefix;
	
	if ( path.isEmpty() ) {
		path = QLatin1String( ":/" );
	}
	
	const pFileNamePair pair = qMakePair( fileName, path );
	QString fn = mFileNameCache.value( pair );
	
	if ( !fn.isEmpty() ) {
		return fn;
	}
	
	QDir dir( path );
	fn = pCoreUtils::findFile( dir, fileName );
	mFileNameCache[ pair ] = fn;
	
	return fn;
}

QPixmap pIconManager::pixmap( const QString& fileName, const QString& prefix )
{
	QPixmap pixmap;
	const QString fn = filePath( fileName, prefix );
	
	if ( !QPixmapCache::find( fn, pixmap ) ) {
		if ( pixmap.load( fn ) ) {
			if ( !QPixmapCache::insert( fn, pixmap ) ) {
				qWarning() << Q_FUNC_INFO << "Can't insert pixmap" << fn;
			}
		}
		else {
			qWarning() << Q_FUNC_INFO << "Can't load pixmap" << fn;
		}
	}
	
	return pixmap;
}

QIcon pIconManager::icon( const QString& fileName, const QString& prefix )
{
	const QString fn = filePath( fileName, prefix );
	QIcon icon;
	
	if ( mIconCache.contains( fn ) ) {
		icon = *mIconCache[ fn ];
	}
	else {
		const QPixmap pixmap = pIconManager::pixmap( fileName, prefix );
		
		if ( !pixmap.isNull() ) {
			icon.addPixmap( pixmap );
			
			if ( !mIconCache.insert( fn, new QIcon( icon ) ) ) {
				qWarning() << Q_FUNC_INFO << "Can't insert icon" << fn;
			}
		}
	}
	
	return icon;
}
