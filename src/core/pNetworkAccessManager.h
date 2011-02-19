/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pNetworkAccessManager.h
** Date      : 2011-02-20T00:44:25
** License   : LGPL v3
** Home Page : http://bettercodes.org/projects/fresh
** Comment   : Fresh Library is a Qt 4 extension library providing set of new core & gui classes.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Leser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
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
	\sa QNetworkAccessManager.
*/
class pNetworkAccessManager : public QNetworkAccessManager
{
	Q_OBJECT
	
public:
	/*!
		Returns the global instance of the pNetworkAccessManager.
	*/
	static pNetworkAccessManager* instance();
	/*!
		Construct a pNetworkAccessManager instance and set the parent to \a parent.
	*/
	pNetworkAccessManager( QObject* parent = 0 );
	/*!
		Destroy the pNetworkAccessManager and free all its resources.
	*/
	virtual ~pNetworkAccessManager();
	/*!
		Set the directory where to cache the QNetworkReply to \a cacheDir.
	*/
	void setCacheDirectory( const QString& cacheDir );
	/*!
		Returns the directory where are cached the QNetworkReply.
		\sa QNetworkDiskCache.
	*/
	QString cacheDirectory() const;
	/*!
		Set the maximum cache size of the cache directory to \a size bytes.
		\sa QNetworkDiskCache.
	*/
	void setMaximumCacheSize( qint64 size );
	/*!
		Returns the maximum size of the cache directory in bytes.
		\sa QNetworkDiskCache.
	*/
	qint64 maximumCacheSize() const;
	/*!
		Set the maximum try an url will be sent to \a max.
	*/
	void setMaxRetryPerUrl( int max );
	/*!
		Returns the maximum number of time an url is allowed to be sent.
		
		If a request is sent and max retry has been reach, then the request will
		not be sent and an error will be triggered.
	*/
	int maxRetryPerUrl() const;
	/*!
		Returns the current cache size in bytes.
		\sa QNetworkDiskCache.
	*/
	qint64 cacheSize() const;
	/*!
		Clear the cache, all cache files are deleted from the disk.
		\sa QNetworkDiskCache.
	*/
	void clearCache();
	/*!
		Remove the cache file for \a url and return true on success, else false.
		\sa QNetworkDiskCache.
	*/
	bool removeCache( const QUrl& url ) const;
	/*!
		Return a QIODevice for the cached data \a url if exists, else 0.
		\sa QNetworkDiskCache.
	*/
	QIODevice* cacheData( const QUrl& url ) const;
	/*!
		Return true if cache data exists for \a url, else false.
		\sa QNetworkDiskCache.
	*/
	bool hasCacheData( const QUrl& url ) const;
	/*!
		Return the meta data associated with \a url.
		\sa QNetworkDiskCache.
	*/
	QNetworkCacheMetaData cacheMetaData( const QUrl& url ) const;
	/*!
		Return the meta data associated with \a fileName.
		\sa QNetworkDiskCache.
	*/
	QNetworkCacheMetaData cacheFileMetaData( const QString& fileName ) const;
	
#if defined( QT_GUI_LIB )
	/*!
		Return a QPixmap from the cache content of \a url if it exists, else an invalid QPixmap.
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
		Reimplemented.
	*/
	virtual QNetworkReply* createRequest( Operation op, const QNetworkRequest& req, QIODevice* outgoingData = 0 );

protected slots:
	void _q_finished( QNetworkReply* reply );

signals:
	/*!
		This signal is emitted when a request at \a url is successfuly cached.
	*/
	void cached( const QUrl& url );
	/*!
		This signal is emitted when a request at \a url fails.
		The error associated is set in \a message.
	*/
	void error( const QUrl& url, const QString& message );
	/*!
		This signal is emmited when the cache has been cleared.
	*/
	void cacheCleared();
};

#endif // PNETWORKACCESSMANAGER_H
