#ifndef PNETWORKACCESSMANAGER_H
#define PNETWORKACCESSMANAGER_H

/*!
    \file pNetworkAccessManager.h
    \brief A QNetworkAccessManager with disk cache and cache settings helpers.
	
	This class allow to use a cache of request with no efforts at all.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include <QNetworkAccessManager>
#include <QNetworkCacheMetaData>
#include <QSet>
#if defined( QT_GUI_LIB )
#include <QPixmap>
#endif

class QNetworkDiskCache;

/*!
	\ingroup Core
    \class pNetworkAccessManager
    \brief A QNetworkAccessManager with disk cache and cache settings helpers.
	
	This class allow to use a cache of request with no efforts at all.
	\sa QNetworkAccessManager
*/
class pNetworkAccessManager : public QNetworkAccessManager
{
	Q_OBJECT
	
public:
	/*!
		\brief Returns the global instance of the pNetworkAccessManager class.
		\return A pNetworkAccessManager.
	*/
	static pNetworkAccessManager* instance();
	/*!
		\brief Construct a pNetworkAccessManager and set the parent to \a parent.
	*/
	pNetworkAccessManager( QObject* parent = 0 );
	/*!
		\brief Destroy the pNetworkAccessManager and free all its resources.
	*/
	virtual ~pNetworkAccessManager();
	/*!
		\brief Set the directory where to cache the QNetworkReply to \a cacheDir.
		\param cacheDir The directory where to cache the QNetworkReply.
	*/
	void setCacheDirectory( const QString& cacheDir );
	/*!
		\brief Returns the directory where are cached the QNetworkReply.
		\return A QString containing the cache directory.
		\sa QNetworkDiskCache
	*/
	QString cacheDirectory() const;
	/*!
		\brief Set the maximum cache size of the cache directory.
		\param size The maximum size in bytes for the cached QNetworkReply.
		\sa QNetworkDiskCache
	*/
	void setMaximumCacheSize( qint64 size );
	/*!
		\brief Returns the maximum size of the cache directory.
		\return A qint64 having the maximum size in bytes.
		\sa QNetworkDiskCache
	*/
	qint64 maximumCacheSize() const;
	/*!
		\brief Set the maximum try an url will be sent.
		\param max The maximum try an url will be sent.
		\sa QNetworkDiskCache
	*/
	void setMaxRetryPerUrl( int max );
	/*!
		\brief Returns the maximum number of time an url is allowed to be sent.
		
		If a request is sent and max retry has been reach, then the request will
		not be sent and an error will be triggered.
		\return An int representing the maximum number of time an url is allowed to be sent.
	*/
	int maxRetryPerUrl() const;
	/*!
		\brief Returns the current cache size.
		\return A qint64 having the size of the current cache in bytes.
	*/
	qint64 cacheSize() const;
	/*!
		\brief Clear the cache, al cache files are deleted from the disk.
		\sa QNetworkDiskCache
	*/
	void clearCache();
	/*!
		\brief Remove the cache file for \a url.
		\return A bool for success or error.
		\sa QNetworkDiskCache
	*/
	bool removeCache( const QUrl& url ) const;
	/*!
		\brief Return a QIODevice for the cached data \a url.
		\param url The url to get cache of.
		\return A QIODevice for the cache or 0 if the cache for \a url does not exists.
		\sa QNetworkDiskCache
	*/
	QIODevice* cacheData( const QUrl& url ) const;
	/*!
		\brief Tell if cache data exists for \a url.
		\param url The url to check cache data exists.
		\return A bool, true if cache exists, else false.
		\sa QNetworkDiskCache
	*/
	bool hasCacheData( const QUrl& url ) const;
	/*!
		\brief Return the meta data associated with \a url.
		\param url The url of the cache data to return meta data.
		\return The QNetworkCacheMetaData associated with \a url.
		\sa QNetworkDiskCache
	*/
	QNetworkCacheMetaData cacheMetaData( const QUrl& url ) const;
	/*!
		\brief Return the meta data associated with \a fileName.
		\param fileName The file name of the cache data to return meta data.
		\return The QNetworkCacheMetaData associated with \a fileName.
		\sa QNetworkDiskCache
	*/
	QNetworkCacheMetaData cacheFileMetaData( const QString& fileName ) const;
	
#if defined( QT_GUI_LIB )
	/*!
		\brief Return a QPixmap from the cache content of \a url.
		\param url The url of the cache data to get QPixmap from.
		\return A QPixmap of the cache data content if available, else an invalid QPixmap.
		\note This member is only defined when the macro QT_GUI_LIB is defined.
	*/
	QPixmap cachedPixmap( const QUrl& url ) const;
#endif

protected:
	QNetworkDiskCache* mCache;
	int mMaxRetryPerUrl;
	QSet<QUrl> mPendingRequests;
	QHash<QUrl, int> mRetryRequests;
	
	/*!
		\brief Reimplemented to handle max retry per url
		\sa QNetworkAccessManager
	*/
	virtual QNetworkReply* createRequest( Operation op, const QNetworkRequest& req, QIODevice* outgoingData = 0 );

protected slots:
	void _q_finished( QNetworkReply* reply );

signals:
	/*!
		\brief This signal is emitted when a request is successfuly cached.
		\param url The url of the cached file.
	*/
	void cached( const QUrl& url );
	/*!
		\brief This signal is emitted when a request fails.
		\param url The url of the request that fails.
		\param message The error message associated with the error.
	*/
	void error( const QUrl& url, const QString& message );
	/*!
		\brief This signal is emmited when the cache has been cleared.
	*/
	void cacheCleared();
};

#endif // PNETWORKACCESSMANAGER_H
