namespace pGuiUtils
{
%TypeHeaderCode
#include <gui/pGuiUtils.h>
%End
	/*!
		Create a QPixmap of \a size filled with \a color.
	*/
	QPixmap filledPixmap( const QColor& color, const QSize& size );
	/*!
		Create a QPixmap from \a filePath. If \a size is not null, the pixmap is scaled to \a size.
		The resulted pixmap is cached in QPixmapCache so a call with same parameters will be very quick.
		\note The scale is done using Qt::KeepAspectRatio and Qt::SmoothTransformation.
	*/
	QPixmap scaledPixmap( const QString& filePath, const QSize& size = QSize() );
	/*!
		Create a QPixmap from \a pixmap. If \a size is not null, the pixmap is scaled to \a size.
		The resulted pixmap is cached in QPixmapCache using \a key so a call with same parameters will be very quick.
		\note The scale is done using Qt::KeepAspectRatio and Qt::SmoothTransformation.
	*/
	QPixmap scaledPixmap( const QPixmap& pixmap, const QString& key, const QSize& size = QSize() );
};
