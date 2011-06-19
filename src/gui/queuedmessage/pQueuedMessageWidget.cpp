/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pQueuedMessageWidget.cpp
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
#include "pQueuedMessageWidget.h"
#include "pIconManager.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include <QStyle>
#include <QPushButton>
#include <QPainter>
#include <QDebug>

pQueuedMessage::pQueuedMessage()
{
	milliSeconds = -1;
	background = QBrush( Qt::NoBrush );
	foreground = QBrush( Qt::NoBrush );
	slot = 0;
}

bool pQueuedMessage::operator==( const pQueuedMessage& other ) const
{
	return
		message == other.message
		&& milliSeconds == other.milliSeconds
		&& ( pixmap.cacheKey() == other.pixmap.cacheKey() || ( pixmap.isNull() && other.pixmap.isNull() ) )
		&& background == other.background
		&& foreground == other.foreground
		&& buttons == other.buttons
		&& object == other.object
		&& slot == other.slot
		&& userData == other.userData
		;
}

pQueuedMessageWidget::pQueuedMessageWidget( QWidget* parent )
	: QWidget( parent )
{
	mDefaultTimeout = 0;
	mDefaultPixmap = pIconManager::pixmap( "info.png", ":/fresh/icons" );
	mDefaultBackground = QBrush( QColor( 250, 230, 147 ) );
	mDefaultForeground = QBrush( QColor( 0, 0, 0 ) );
	
	// pixmap
	lPixmap = new QLabel( this );
	lPixmap->setAlignment( Qt::AlignCenter );
	lPixmap->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
	
	// message
	lMessage = new QLabel( this );
	lMessage->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
	lMessage->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred ) );
	lMessage->setWordWrap( true );
	lMessage->setOpenExternalLinks( true );
	lMessage->setTextInteractionFlags( Qt::TextBrowserInteraction );
	
	// button
	dbbButtons = new QDialogButtonBox( this );
	dbbButtons->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
	
	setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
	
	// layout
	QHBoxLayout* hbl = new QHBoxLayout( this );
	hbl->setMargin( 0 );
	hbl->addWidget( lPixmap, 0, Qt::AlignCenter );
	hbl->addWidget( lMessage );
	hbl->addWidget( dbbButtons, 0, Qt::AlignCenter );
	
	// font
	QFont font = this->font();
#if defined( Q_OS_MAC )
	font.setPointSize( 11 );
#else
	font.setPointSize( 9 );
#endif
	setFont( font );
	
	// connections
	connect( lMessage, SIGNAL( linkActivated( const QString& ) ), this, SIGNAL( linkActivated( const QString& ) ) );
	connect( lMessage, SIGNAL( linkHovered( const QString& ) ), this, SIGNAL( linkHovered( const QString& ) ) );
	connect( dbbButtons, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( buttonClicked( QAbstractButton* ) ) );
}

QSize pQueuedMessageWidget::sizeHint() const
{
	return QWidget::minimumSizeHint();
}

bool pQueuedMessageWidget::openExternalLinks() const
{
	return lMessage->openExternalLinks();
}

int pQueuedMessageWidget::defaultTimeout() const
{
	return mDefaultTimeout;
}

QPixmap pQueuedMessageWidget::defaultPixmap() const
{
	return mDefaultPixmap;
}

QBrush pQueuedMessageWidget::defaultBackground() const
{
	return mDefaultBackground;
}

QBrush pQueuedMessageWidget::defaultForeground() const
{
	return mDefaultForeground;
}

void pQueuedMessageWidget::currentMessageInformations( QPixmap* pixmap, QBrush* background, QBrush* foreground ) const
{
	if ( pixmap ) {
		*pixmap = currentMessagePixmap();
	}
	
	if ( background ) {
		*background = currentMessageBackground();
	}
	
	if ( foreground ) {
		*foreground = currentMessageForeground();
	}
}

int pQueuedMessageWidget::pendingMessageCount() const
{
	return mMessages.count();
}

pQueuedMessage pQueuedMessageWidget::currentMessage() const
{
	return mMessages.value( 0 );
}

pQueuedMessage pQueuedMessageWidget::append( const QString& message, int milliSeconds )
{
	pQueuedMessage msg;
	msg.message = message;
	msg.milliSeconds = milliSeconds == -1 ? mDefaultTimeout : milliSeconds;
	msg.pixmap = mDefaultPixmap;
	msg.background = mDefaultBackground;
	msg.foreground = mDefaultForeground;
	
	append( msg );
	
	return msg;
}

void pQueuedMessageWidget::setOpenExternalLinks( bool open )
{
	lMessage->setOpenExternalLinks( open );
}

void pQueuedMessageWidget::setDefaultTimeout( int timeout )
{
	mDefaultTimeout = timeout;
}

void pQueuedMessageWidget::setDefaultPixmap( const QPixmap& pixmap )
{
	mDefaultPixmap = pixmap;
}

void pQueuedMessageWidget::setDefaultBackground( const QBrush& brush )
{
	mDefaultBackground = brush;
}

void pQueuedMessageWidget::setDefaultForeground( const QBrush& brush )
{
	mDefaultForeground = brush;
}

void pQueuedMessageWidget::append( const pQueuedMessage& message )
{
	if ( !mMessages.contains( message ) ) {
		mMessages << message;
		
		if ( mMessages.count() == 1 ) {
			QTimer::singleShot( 0, this, SLOT( showMessage() ) );
		}
	}
}

void pQueuedMessageWidget::remove( const pQueuedMessage& message )
{
	if ( mMessages.isEmpty() || mMessages.first() == message ) {
		return;
	}
	
	mMessages.removeOne( message );
}

void pQueuedMessageWidget::clear()
{
	mMessages.clear();
	lPixmap->clear();
	lMessage->clear();
	dbbButtons->clear();
	emit cleared();
}

QPixmap pQueuedMessageWidget::currentMessagePixmap() const
{
	const pQueuedMessage msg = currentMessage();
	return msg.pixmap.isNull() ? mDefaultPixmap : msg.pixmap;
}

QBrush pQueuedMessageWidget::currentMessageBackground() const
{
	const pQueuedMessage msg = currentMessage();
	return msg.background == QBrush( Qt::NoBrush ) ? mDefaultBackground : msg.background;
}

QBrush pQueuedMessageWidget::currentMessageForeground() const
{
	const pQueuedMessage msg = currentMessage();
	return msg.foreground == QBrush( Qt::NoBrush ) ? mDefaultForeground : msg.foreground;
}

void pQueuedMessageWidget::paintEvent( QPaintEvent* event )
{
	if ( pendingMessageCount() == 0 ) {
		QWidget::paintEvent( event );
		return;
	}
	
	QPainter painter( this );
	painter.setPen( Qt::NoPen );
	painter.setBrush( currentMessageBackground() );
	painter.drawRect( contentsRect() );
}

void pQueuedMessageWidget::buttonClicked( QAbstractButton* button )
{
	const pQueuedMessage msg = currentMessage();
	const QDialogButtonBox::StandardButton standardButton = dbbButtons->standardButton( button );
	
	if ( msg.object && msg.slot ) {
		QMetaObject::invokeMethod( msg.object.data(), msg.slot, Q_ARG( QDialogButtonBox::StandardButton, standardButton ), Q_ARG( pQueuedMessage, msg ) );
	}
	
	closeMessage();
}

void pQueuedMessageWidget::showMessage()
{
	// get message
	pQueuedMessage msg = currentMessage();
	
	// update palette
	QPalette pal = lMessage->palette();
	pal.setBrush( lMessage->foregroundRole(), currentMessageForeground() );
	lMessage->setPalette( pal );
	
	// format widget
	lPixmap->setPixmap( currentMessagePixmap() );
	lMessage->setText( msg.message );
	lMessage->setToolTip( msg.message );
	lMessage->setWhatsThis( msg.message );
	
	// set buttons
	if ( msg.buttons.isEmpty() ) {
		msg.buttons[ QDialogButtonBox::Ok ] = QString::null;
	}
	
	dbbButtons->clear();
	
	foreach( const QDialogButtonBox::StandardButton& button, msg.buttons.keys() ) {
		QPushButton* pb = dbbButtons->addButton( button );
		
		if ( !msg.buttons[ button ].isEmpty() ) {
			pb->setText( msg.buttons[ button ] );
		}
	}
	
	// auto close if needed
	const int timeout = msg.milliSeconds == -1 ? mDefaultTimeout : msg.milliSeconds;
	
	if ( timeout > 0 ) {
		QTimer::singleShot( timeout, this, SLOT( closeMessage() ) );
	}
	
	// emit signal
	emit shown( msg );
}

void pQueuedMessageWidget::closeMessage()
{
	// emit message
	emit closed( currentMessage() );
	
	// remove remove current message from hash
	mMessages.removeFirst();
	
	// process next if possible, else clear gui
	QTimer::singleShot( 0, this, mMessages.count() > 0 ? SLOT( showMessage() ) : SLOT( clearMessage() ) );
	
	// emit finished message if needed
	if ( mMessages.isEmpty() ) {
		emit finished();
	}
}

void pQueuedMessageWidget::clearMessage()
{
	lPixmap->clear();
	lMessage->clear();
	dbbButtons->clear();
}
