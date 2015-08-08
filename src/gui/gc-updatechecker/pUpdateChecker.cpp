/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pUpdateChecker.cpp
** Date      : 2011-02-20T00:41:09
** License   : LGPL v3
** Home Page : https://github.com/pasnox/fresh
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
#include "pUpdateChecker.h"
#include "pUpdateCheckerDialog.h"
#include "pIconManager.h"

#include <QTimer>
#include <QAction>
#include <QEvent>

pUpdateChecker::pUpdateChecker( QWidget* parent )
    : QObject( parent )
{
    Q_ASSERT( parent );
    mParent = parent;

    mAction = new QAction( this );
    mAction->setIcon( pIconManager::pixmap( QSL( "gc-update-checker.png" ), QSL( ":/fresh/icons" ) ) );

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
