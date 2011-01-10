#include "pUpdateChecker.h"
#include "pUpdateCheckerDialog.h"
#include "gui/pIconManager.h"

#include <QTimer>
#include <QAction>
#include <QEvent>

pUpdateChecker::pUpdateChecker( QWidget* parent )
	: QObject( parent )
{
	Q_ASSERT( parent );
	mParent = parent;
	
	mAction = new QAction( this );
	mAction->setIcon( pIconManager::pixmap( "gc-update-checker.png", ":/fresh/icons" ) );
	
	localeChanged();
	
	mParent.data()->installEventFilter( this );
	
	connect( mAction, SIGNAL( triggered() ), this, SLOT( checkForUpdate_triggered() ) );
}

bool pUpdateChecker::eventFilter( QObject* object, QEvent* event )
{
	switch ( event->type() ) {
		case QEvent::LocaleChange:
			localeChanged();
			break;
		default:
			break;
	}
	
	return QObject::eventFilter( object, event );
}

QAction* pUpdateChecker::menuAction() const
{
	return mAction;
}

QDateTime pUpdateChecker::lastUpdated() const
{
	return mLastUpdated;
}

void pUpdateChecker::setLastUpdated( const QDateTime& dateTime )
{
	mLastUpdated = dateTime;
}

QDateTime pUpdateChecker::lastChecked() const
{
	return mLastChecked;
}

void pUpdateChecker::setLastChecked( const QDateTime& dateTime )
{
	mLastChecked = dateTime;
}

QUrl pUpdateChecker::downloadsFeedUrl() const
{
	return mDownloadsFeed;
}

void pUpdateChecker::setDownloadsFeedUrl( const QUrl& url )
{
	mDownloadsFeed = url;
}

QString pUpdateChecker::version() const
{
	return mVersion;
}

void pUpdateChecker::setVersion( const QString& version )
{
	mVersion = version;
}

QString pUpdateChecker::versionString() const
{
	return mVersionString;
}

void pUpdateChecker::setVersionString( const QString& versionString )
{
	mVersionString = versionString;
}

QString pUpdateChecker::versionDiscoveryPattern() const
{
	return mVersionDiscoveryPattern;
}

void pUpdateChecker::setVersionDiscoveryPattern( const QString& pattern )
{
	mVersionDiscoveryPattern = pattern;
}

void pUpdateChecker::silentCheck()
{
	checkForUpdate_triggered( false );
}

void pUpdateChecker::localeChanged()
{
	mAction->setText( tr( "Update" ) );
	mAction->setToolTip( tr( "Check for update" ) );
}

void pUpdateChecker::checkForUpdate_triggered( bool show )
{
	pUpdateCheckerDialog* dlg = new pUpdateCheckerDialog( this, mParent.data() );
	
	if ( show ) {
		dlg->open();
	}
}
