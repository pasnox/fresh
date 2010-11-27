#include "pNetworkAccessManager.h"

#include <QCoreApplication>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QDebug>

#if defined( QT_GUI_LIB )
#include <QPixmapCache>
#endif

Q_GLOBAL_STATIC( pNetworkAccessManager, networkAccessManager );

pNetworkAccessManager* pNetworkAccessManager::instance()
{
	return networkAccessManager();
}

pNetworkAccessManager::pNetworkAccessManager( QObject* parent )
	: QNetworkAccessManager( parent )
{
	mCache = new QNetworkDiskCache( this );
	setCache( mCache );
	
	connect( this, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( _q_finished( QNetworkReply* ) ) );
}

pNetworkAccessManager::~pNetworkAccessManager()
{
}

void pNetworkAccessManager::_q_finished( QNetworkReply* reply )
{
	if ( reply->error() == QNetworkReply::NoError ) {
		emit cached( reply->url() );
	}
	else {
		emit error( reply->url(), reply->errorString() );
	}
}

void pNetworkAccessManager::setCacheDirectory( const QString& cacheDir )
{
	mCache->setCacheDirectory( cacheDir );
}

QString pNetworkAccessManager::cacheDirectory() const
{
	return mCache->cacheDirectory();
}

void pNetworkAccessManager::setMaximumCacheSize( qint64 size )
{
	mCache->setMaximumCacheSize( size );
}

qint64 pNetworkAccessManager::maximumCacheSize() const
{
	return mCache->maximumCacheSize();
}

qint64 pNetworkAccessManager::cacheSize() const
{
	return mCache->cacheSize();
}

void pNetworkAccessManager::clearCache()
{
	mCache->clear();
	emit cacheCleared();
}

bool pNetworkAccessManager::removeCache( const QUrl& url ) const
{
	return mCache->remove( url );
}

QIODevice* pNetworkAccessManager::cacheData( const QUrl& url ) const
{
	return mCache->data( url );
}

bool pNetworkAccessManager::hasCacheData( const QUrl& url ) const
{
	const QIODevice* data = mCache->data( url );
	const bool has = data;
	delete data;
	return has;
}

QNetworkCacheMetaData pNetworkAccessManager::cacheMetaData( const QUrl& url ) const
{
	return mCache->metaData( url );
}

QNetworkCacheMetaData pNetworkAccessManager::cacheFileMetaData( const QString& fileName ) const
{
	return mCache->fileMetaData( fileName );
}

#if defined( QT_GUI_LIB )
QPixmap pNetworkAccessManager::cachedPixmap( const QUrl& url ) const
{
	const QString key = url.toString();
	QPixmap pixmap;
	
	if ( !QPixmapCache::find( key, pixmap ) ) {
		QIODevice* data = mCache->data( url );
		
		if ( data ) {
			if ( pixmap.loadFromData( data->readAll() ) ) {
				if ( !QPixmapCache::insert( key, pixmap ) ) {
					qWarning() << Q_FUNC_INFO << "Can't cache pixmap" << key;
				}
			}
			else {
				qWarning() << Q_FUNC_INFO << "Can't load pixmap to cache" << key;
			}
		}
	}
	
	return pixmap;
}
#endif

