#ifndef PDRAWINGUTILS_H
#define PDRAWINGUTILS_H

/*!
	\file pDrawingUtils.h
	\brief A central place for reusable drawing functions.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"

#include <QPixmap>

/*!
	\ingroup Gui
	\namespace pDrawingUtils
	\brief A central place for reusable drawing functions.
*/
namespace pDrawingUtils
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

#endif // PDRAWINGUTILS_H
