/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pPaypalButton.cpp
** Date      : 2011-02-20T00:41:35
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
#include "pPaypalButton.h"
#include "pNetworkAccessManager.h"
#include "pIconManager.h"

#include <QEvent>
#include <QDesktopServices>
#include <QLocale>
#include <QStyleOptionButton>
#include <QPainter>
#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif
#include <QDebug>

pPaypalButton::pPaypalButton( QWidget* parent )
    : QPushButton( parent )
{
    setCursor( Qt::PointingHandCursor );
    setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum ) );

    mQueryItems[ "path" ] = QString( "%1/cgi-bin/webscr" ).arg( PAYPAL_DOMAIN );
    mQueryItems[ "cmd" ] = "_donations";
    mQueryItems[ "bn" ] = QUrl::fromPercentEncoding( "PP%2dDonationsBF%3abtn_donate_SM%2egif%3aNonHosted" );
    mAutoOpenUrl = true;

    localeChanged();

    connect( this, SIGNAL( clicked() ), this, SLOT( _q_clicked() ) );
    connect( pNetworkAccessManager::instance(), SIGNAL( cached( const QUrl& ) ), this, SLOT( networkAccessManager_cached( const QUrl& ) ) );
    connect( pNetworkAccessManager::instance(), SIGNAL( error( const QUrl&, const QString& ) ), this, SLOT( networkAccessManager_error( const QUrl&, const QString& ) ) );
    connect( pNetworkAccessManager::instance(), SIGNAL( cacheCleared() ), this, SLOT( networkAccessManager_cacheCleared() ) );
}

QSize pPaypalButton::minimumSizeHint() const
{
    return sizeHint();
}

QSize pPaypalButton::sizeHint() const
{
    return mPixmap.isNull() ? QPushButton::sizeHint() : mPixmap.size() +QSize( 1, 1 );
}

bool pPaypalButton::event( QEvent* event )
{
    switch ( event->type() ) {
        case QEvent::LocaleChange:
            localeChanged();
            updatePixmap();
            break;
        default:
            break;
    }

    return QPushButton::event( event );
}

void pPaypalButton::paintEvent( QPaintEvent* event )
{
    if ( mPixmap.isNull() ) {
        QPushButton::paintEvent( event );
    }
    else {
        QPainter painter( this );

        QStyleOptionButton option;
        initStyleOption( &option );
        option.rect = option.rect.adjusted( 0, 0, -1, -1 );
        option.iconSize = size();
        option.icon = mPixmap;
        option.text.clear();

        style()->drawControl( QStyle::CE_PushButtonLabel, &option, &painter, this );
    }
}

QUrl pPaypalButton::pixmapUrl( const QString& locale )
{
    return QUrl( QString( PAYPAL_MASK ).arg( locale ) );
}

void pPaypalButton::updatePixmap()
{
    const QUrl url = pixmapUrl( locale().name() );

    networkAccessManager_cached( url );

    if ( !pNetworkAccessManager::instance()->hasCacheData( url ) ) {
        pNetworkAccessManager::instance()->get( QNetworkRequest( url ) );
    }
}

QString pPaypalButton::actionPost() const
{
    return mQueryItems.value( "path" );
}

void pPaypalButton::setActionPost( const QString& value )
{
    mQueryItems[ "path" ] = value;
}

QString pPaypalButton::businessId() const
{
    return mQueryItems.value( "business" );
}

void pPaypalButton::setBusinessId( const QString& value )
{
    mQueryItems[ "business" ] = value;
}

QString pPaypalButton::itemName() const
{
    return mQueryItems.value( "item_name" );
}

void pPaypalButton::setItemName( const QString& value )
{
    mQueryItems[ "item_name" ] = value;
}

QString pPaypalButton::itemId() const
{
    return mQueryItems.value( "item_number" );
}

void pPaypalButton::setItemId( const QString& value )
{
    mQueryItems[ "item_number" ] = value;
}

QString pPaypalButton::currencyCode() const
{
    return mQueryItems.value( "currency_code" );
}

void pPaypalButton::setCurrencyCode( const QString& value )
{
    mQueryItems[ "currency_code" ] = value;
}

bool pPaypalButton::autoOpenUrl() const
{
    return mAutoOpenUrl;
}

void pPaypalButton::setAutoOpenUrl( bool open )
{
    mAutoOpenUrl = open;
}

QPixmap pPaypalButton::pixmap( const QUrl& url ) const
{
    QPixmap pixmap = pNetworkAccessManager::instance()->cachedPixmap( url );

    if ( pixmap.isNull() ) {
        pixmap = pIconManager::pixmap( "paypal.png", ":/fresh/icons" );
    }

    return pixmap;
}

QUrl pPaypalButton::url() const
{
    QUrl url( mQueryItems.value( "path" ) );
    QList<QPair<QString, QString> > queryItems;

    foreach ( const QString& key, mQueryItems.keys() ) {
        if ( key == "path" ) {
            continue;
        }

        queryItems << qMakePair( key, mQueryItems[ key ] );
    }

#if QT_VERSION < 0x050000
    url.setQueryItems( queryItems );
#else
    QUrlQuery query;
    query.setQueryItems( queryItems );
    url.setQuery( query );
#endif

    return url;
}

void pPaypalButton::localeChanged()
{
    mQueryItems[ "lc" ] = locale().name().section( "_", 1 );

    setText( tr( "Donation" ) );
    setToolTip( tr( "Make a donation via Paypal" ) );
}

void pPaypalButton::_q_clicked()
{
    if ( mAutoOpenUrl ) {
        QDesktopServices::openUrl( url() );
    }
    else {
        emit clicked( url() );
    }
}

void pPaypalButton::networkAccessManager_cached( const QUrl& url )
{
    if ( url.toString().startsWith( PAYPAL_DOMAIN, Qt::CaseInsensitive ) ) {
        mPixmap = this->pixmap( url );
        updateGeometry();
    }
}

void pPaypalButton::networkAccessManager_error( const QUrl& url, const QString& message )
{
    if ( url.toString().startsWith( PAYPAL_DOMAIN, Qt::CaseInsensitive ) ) {
        qWarning() << message << url;
    }
}

void pPaypalButton::networkAccessManager_cacheCleared()
{
    updatePixmap();
}
