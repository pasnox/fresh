#include "pDrawingUtils.h"

#include <QPixmapCache>
#include <QDebug>

QPixmap pDrawingUtils::filledPixmap( const QColor& color, const QSize& size )
{
	const QString key = QString( "pDrawingUtils::filledPixmap-%1-%2-%3-%4" )
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
