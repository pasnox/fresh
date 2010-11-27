#ifndef PUPDATECHECKER_H
#define PUPDATECHECKER_H

#include "core/FreshExport.h"

#include <QObject>
#include <QWeakPointer>
#include <QDateTime>
#include <QUrl>

class QAction;

class FRESH_EXPORT pUpdateChecker : public QObject
{
	Q_OBJECT

public:
	pUpdateChecker( QWidget* parent = 0 );
	
	QAction* menuAction() const;
	QDateTime lastUpdated() const;
	QDateTime lastChecked() const;
	QUrl downloadsFeedUrl() const;
	QString version() const;
	QString versionString() const;
	QString versionDiscoveryPattern() const;

public slots:
	void setLastUpdated( const QDateTime& dateTime );
	void setLastChecked( const QDateTime& dateTime );
	void setDownloadsFeedUrl( const QUrl& url );
	void setVersion( const QString& version );
	void setVersionString( const QString& versionString );
	void setVersionDiscoveryPattern( const QString& pattern );
	void silentCheck();

protected:
	QAction* mAction;
	QDateTime mLastUpdated;
	QDateTime mLastChecked;
	QUrl mDownloadsFeed;
	QString mVersion;
	QString mVersionString;
	QString mVersionDiscoveryPattern;
	QWeakPointer<QWidget> mParent;
	
	virtual bool eventFilter( QObject* object, QEvent* event );
	virtual void localeChanged();
	
protected slots:
	void checkForUpdate_triggered( bool show = true );
};

#endif // PUPDATECHECKER_H