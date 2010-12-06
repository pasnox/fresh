#ifndef PNETWORKACCESSMANAGER_H
#define PNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkCacheMetaData>
#include <QSet>
#if defined( QT_GUI_LIB )
#include <QPixmap>
#endif

class QNetworkDiskCache;

class pNetworkAccessManager : public QNetworkAccessManager
{
	Q_OBJECT
	
public:
	static pNetworkAccessManager* instance();
	pNetworkAccessManager( QObject* parent = 0 );
	virtual ~pNetworkAccessManager();
	
	void setCacheDirectory( const QString& cacheDir );
	QString cacheDirectory() const;
	
	void setMaximumCacheSize( qint64 size );
	qint64 maximumCacheSize() const;
	
	void setMaxRetryPerUrl( int max );
	int maxRetryPerUrl() const;
	
	qint64 cacheSize() const;
	void clearCache();
	
	bool removeCache( const QUrl& url ) const;
	QIODevice* cacheData( const QUrl& url ) const;
	bool hasCacheData( const QUrl& url ) const;
	QNetworkCacheMetaData cacheMetaData( const QUrl& url ) const;
	QNetworkCacheMetaData cacheFileMetaData( const QString& fileName ) const;
	
#if defined( QT_GUI_LIB )
	QPixmap cachedPixmap( const QUrl& url ) const;
#endif

protected:
	QNetworkDiskCache* mCache;
	int mMaxRetryPerUrl;
	QSet<QUrl> mPendingRequests;
	QHash<QUrl, int> mRetryRequests;
	
	virtual QNetworkReply* createRequest( Operation op, const QNetworkRequest& req, QIODevice* outgoingData = 0 );

protected slots:
	void _q_finished( QNetworkReply* reply );

signals:
	void cached( const QUrl& url );
	void error( const QUrl& url, const QString& message );
	void cacheCleared();
};

#endif // PNETWORKACCESSMANAGER_H
