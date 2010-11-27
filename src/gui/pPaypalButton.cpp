#include "pPaypalButton.h"
#include "core/pNetworkAccessManager.h"
#include "gui/pIconManager.h"

#include <QEvent>
#include <QDesktopServices>
#include <QLocale>
#include <QStyleOptionButton>
#include <QPainter>
#include <QDebug>

pPaypalButton::pPaypalButton( QWidget* parent )
	: QPushButton( parent )
{
	setCursor( Qt::PointingHandCursor );
	setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum ) );
	
	mQueryItems[ "path" ] = "https://www.paypal.com/cgi-bin/webscr";
	mQueryItems[ "cmd" ] = "_donations";
	mQueryItems[ "bn" ] = QUrl::fromPercentEncoding( "PP%2dDonationsBF%3abtn_donate_SM%2egif%3aNonHosted" );
	mAutoOpenUrl = true;
	
	localeChanged();
	
	connect( this, SIGNAL( clicked() ), this, SLOT( _q_clicked() ) );
	connect( pNetworkAccessManager::instance(), SIGNAL( cached( const QUrl& ) ), this, SLOT( networkAccessManager_cached( const QUrl& ) ) );
	connect( pNetworkAccessManager::instance(), SIGNAL( error( const QUrl&, const QString& ) ), this, SLOT( networkAccessManager_error( const QUrl&, const QString& ) ) );
	connect( pNetworkAccessManager::instance(), SIGNAL( cacheCleared() ), this, SLOT( networkAccessManager_cacheCleared() ) );
}

pPaypalButton::~pPaypalButton()
{
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
		case QEvent::Show:
		case QEvent::LocaleChange:
			localeChanged();
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
	return QUrl( QString( "https://www.paypal.com/%1/i/btn/btn_donate_LG.gif" ).arg( locale ) );
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
	
	url.setQueryItems( queryItems );
	
	return url;
}

void pPaypalButton::localeChanged()
{
	mQueryItems[ "lc" ] = locale().name().section( "_", 1 );
	
	setText( tr( "Donation" ) );
	setToolTip( tr( "Make a donation via Paypal" ) );
	
	updatePixmap();
}

void pPaypalButton::updatePixmap()
{
	const QUrl url = pixmapUrl( locale().name() );
	
	networkAccessManager_cached( url );
	
	if ( !pNetworkAccessManager::instance()->hasCacheData( url ) ) {
		pNetworkAccessManager::instance()->get( QNetworkRequest( url ) );
	}
}

void pPaypalButton::_q_clicked()
{
	if ( mAutoOpenUrl ) {
		QDesktopServices::openUrl( url() );
	}
}

void pPaypalButton::networkAccessManager_cached( const QUrl& url )
{
	mPixmap = this->pixmap( url );
	update();
}

void pPaypalButton::networkAccessManager_error( const QUrl& url, const QString& message )
{
	qWarning() << message << url;
}

void pPaypalButton::networkAccessManager_cacheCleared()
{
	updatePixmap();
}
