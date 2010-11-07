#include "pIconManager.h"
#include "core/FileSystemUtils.h"

#include <QPixmapCache>
#include <QDir>

namespace pIconManager {
	pIconManager::QIconCache mIconCache( 200 );
	pIconManager::FileNameCache mFileNameCache;
};

QString pIconManager::filePath( const QString& fileName, const QString& prefix )
{
	QString path = prefix;
	
	if ( path.isEmpty() ) {
		path = QLatin1String( ":/" );
	}
	
	const FileNamePair pair = qMakePair( fileName, path );
	QString fn = mFileNameCache.value( pair );
	
	if ( !fn.isEmpty() ) {
		return fn;
	}
	
	QDir dir( path );
	fn = FileSystemUtils::findFile( dir, fileName );
	mFileNameCache[ pair ] = fn;
	
	return fn;
}

QPixmap pIconManager::pixmap( const QString& fileName, const QString& prefix )
{
	QPixmap pixmap;
	const QString fn = filePath( fileName, prefix );
	
	if ( !QPixmapCache::find( fn, pixmap ) ) {
		if ( pixmap.load( fn ) ) {
			QPixmapCache::insert( fn, pixmap );
		}
	}
	
	return pixmap;
}

QIcon pIconManager::icon( const QString& fileName, const QString& prefix )
{
	QIcon* icon = 0;
	const QString fn = filePath( fileName, prefix );
	
	if ( mIconCache.contains( fn ) ) {
		icon = mIconCache[ fn ];
	}
	else {
		icon = new QIcon( pixmap( fileName, prefix ) );
		
		if ( icon->isNull() ) {
			delete icon;
			icon = 0;
		}
		else {
			mIconCache.insert( fn, icon );
		}
	}
	
	return icon ? *icon : QIcon();
}
