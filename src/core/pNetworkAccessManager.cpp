#include "pNetworkAccessManager.h"

#include <QCoreApplication>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QDebug>

#if defined( QT_GUI_LIB )
#include <QPixmapCache>
#endif

Q_GLOBAL_STATIC( pNetworkAccessManager, networkAccessManager );

uint qHash( const QUrl& url )
{
	return qHash( url.toString() );
}

pNetworkAccessManager* pNetworkAccessManager::instance()
{
	return networkAccessManager();
}

pNetworkAccessManager::pNetworkAccessManager( QObject* parent )
	: QNetworkAccessManager( parent )
{
	mMaxRetryPerUrl = 5;
	mCache = new QNetworkDiskCache( this );
	setCache( mCache );
	
	connect( this, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( _q_finished( QNetworkReply* ) ) );
}

pNetworkAccessManager::~pNetworkAccessManager()
{
}

QNetworkReply* pNetworkAccessManager::createRequest( Operation op, const QNetworkRequest& req, QIODevice* outgoingData )
{
	if ( mPendingRequests.contains( req.url() ) || mRetryRequests[ req.url() ] >= 5 ) {
		QNetworkReply* reply = QNetworkAccessManager::createRequest( QNetworkAccessManager::HeadOperation, QNetworkRequest(), outgoingData );
		return reply;
	}
	
	mPendingRequests << req.url();
	mRetryRequests[ req.url() ]++;
	
	return QNetworkAccessManager::createRequest( op, req, outgoingData );
}

void pNetworkAccessManager::_q_finished( QNetworkReply* reply )
{
	mPendingRequests.remove( reply->request().url() );
	
	if ( reply->operation() == QNetworkAccessManager::HeadOperation && reply->request().url().isEmpty() ) {
		reply->deleteLater();
		return;
	}
	
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

void pNetworkAccessManager::setMaxRetryPerUrl( int max )
{
	mMaxRetryPerUrl = max;
}

int pNetworkAccessManager::maxRetryPerUrl() const
{
	return mMaxRetryPerUrl;
}

qint64 pNetworkAccessManager::cacheSize() const
{
	return mCache->cacheSize();
}

void pNetworkAccessManager::clearCache()
{
	mRetryRequests.clear();
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
			
			delete data;
		}
	}
	
	return pixmap;
}
#endif

