/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pGuiUtils.h
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
#ifndef PGUIUTILS_H
#define PGUIUTILS_H

/*!
	\file pGuiUtils.h
	\brief A central place for reusable gui functions.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QPixmap>

/*!
	\ingroup FreshGui
	\namespace pGuiUtils
	\brief A central place for reusable gui functions.
*/
namespace pGuiUtils
{
	/*!
		Create a QPixmap of \a size filled with \a color.
	*/
	FRESH_EXPORT QPixmap filledPixmap( const QColor& color, const QSize& size );
	/*!
		Create a QPixmap from \a filePath. If \a size is not null, the pixmap is scaled to \a size.
		The resulted pixmap is cached in QPixmapCache so a call with same parameters will be very quick.
		\note The scale is done using Qt::KeepAspectRatio and Qt::SmoothTransformation.
	*/
	FRESH_EXPORT QPixmap scaledPixmap( const QString& filePath, const QSize& size = QSize() );
	/*!
		Create a QPixmap from \a pixmap. If \a size is not null, the pixmap is scaled to \a size.
		The resulted pixmap is cached in QPixmapCache using \a key so a call with same parameters will be very quick.
		\note The scale is done using Qt::KeepAspectRatio and Qt::SmoothTransformation.
	*/
	FRESH_EXPORT QPixmap scaledPixmap( const QPixmap& pixmap, const QString& key, const QSize& size = QSize() );
};

#endif // PGUIUTILS_H