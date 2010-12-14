#include "pIconManager.h"
#include "core/pFileSystemUtils.h"

#include <QPixmapCache>
#include <QDir>
#include <QDebug>

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
	fn = pFileSystemUtils::findFile( dir, fileName );
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
			if ( !mIconCache.insert( fn, new QIcon( pixmap ) ) ) {
				qWarning() << Q_FUNC_INFO << "Can't insert icon" << fn;
			}
		}
	}
	
	return icon;
}
