class pUpdateChecker : QObject
{
%TypeHeaderCode
#include <gui/gc-updatechecker/pUpdateChecker.h>
%End

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
	virtual bool eventFilter( QObject* object, QEvent* event );
	virtual void localeChanged();
	
protected slots:
	void checkForUpdate_triggered( bool show = true );
};
