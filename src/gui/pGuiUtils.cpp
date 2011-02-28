/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pGuiUtils.cpp
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
#include "pGuiUtils.h"

#include <QPixmapCache>
#include <QDebug>

QPixmap pGuiUtils::filledPixmap( const QColor& color, const QSize& size )
{
	const QString key = QString( "pGuiUtils::filledPixmap-%1-%2-%3-%4" )
		.arg( color.name() ).arg( color.alpha() )
		.arg( size.width() ).arg( size.height() );
	QPixmap pixmap;

	if ( !QPixmapCache::find( key, pixmap ) ) {
		pixmap = QPixmap( size );
		pixmap.fill( color );

		if ( !QPixmapCache::insert( key, pixmap ) ) {
			qWarning() << Q_FUNC_INFO << "Can't cache pixmap" << color << size;
		}
	}

	return pixmap;
}

QString pGuiUtils::cacheKey( const QString& key, const QSize& size )
{
	return QString( "%1-%2-%3" ).arg( key ).arg( size.width() ).arg( size.height() );;
}

QPixmap pGuiUtils::scaledPixmap( const QString& filePath, const QSize& size )
{
	const QString key = cacheKey( filePath, size );
	QPixmap pixmap;

	if ( !QPixmapCache::find( key, pixmap ) ) {
		if ( pixmap.load( filePath ) ) {

			if ( size != QSize() ) {
				pixmap = pixmap.scaled( size, Qt::KeepAspectRatio, Qt::SmoothTransformation );
			}

			if ( !QPixmapCache::insert( key, pixmap ) ) {
				qWarning() << Q_FUNC_INFO << "Can't cache pixmap" << filePath;
			}
		}
		else {
			qWarning() << Q_FUNC_INFO << "Pixmap not exists" << filePath;
		}
	}

	return pixmap;
}

QPixmap pGuiUtils::scaledPixmap( const QPixmap& _pixmap, const QString& _key, const QSize& size )
{
	const QString key = cacheKey( _key, size );
	QPixmap pixmap;

	if ( !QPixmapCache::find( key, pixmap ) ) {
		if ( !_pixmap.isNull() ) {

			if ( size != QSize() ) {
				pixmap = _pixmap.scaled( size, Qt::KeepAspectRatio, Qt::SmoothTransformation );
			}
			else {
				pixmap = _pixmap;
			}

			if ( !QPixmapCache::insert( key, pixmap ) ) {
				qWarning() << Q_FUNC_INFO << "Can't cache pixmap" << key;
			}
		}
		else {
			qWarning() << Q_FUNC_INFO << "Pixmap invalid" << key;
		}
	}

	return pixmap;
}
