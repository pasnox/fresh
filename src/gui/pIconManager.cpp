#include "pIconManager.h"

#include <QApplication>
#include <QDir>

namespace pIconManager {
	QIconCache mIconCache( 200 );
	FileNameCache mFileNameCache;
};

QString findFile( QDir& dir, const QString& fileName )
{
	foreach ( const QFileInfo& fi, dir.entryInfoList( QStringList( fileName ), QDir::Files | QDir::CaseSensitive ) ) {
		if ( fi.fileName() == fileName ) {
			return fi.canonicalFilePath();
		}
	}
	
	foreach ( const QFileInfo& fi, dir.entryInfoList( QDir::AllDirs ) ) {
		dir.setPath( fi.canonicalFilePath() );
		const QString fn = findFile( dir, fileName );
		
		if ( !fn.isNull() ) {
			return fn;
		}
	}
	
	return QString::null;
}

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
	fn = findFile( dir, fileName );
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
