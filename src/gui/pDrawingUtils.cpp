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

QPixmap pDrawingUtils::scaledPixmap( const QString& filePath, const QSize& size )
{
	const QString key = QString( "%1-%2-%3" ).arg( filePath ).arg( size.width() ).arg( size.height() );
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
