#include "pQueuedMessageWidget.h"
#include "gui/pIconManager.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include <QStyle>
#include <QPushButton>
#include <QPainter>

/*!
	\details Create a new pQueuedMessageWidget object
	\param parent The parent widget
*/
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
	lMessage->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
	lMessage->setWordWrap( true );
	
	// button
	dbbButtons = new QDialogButtonBox( this );
	dbbButtons->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
	
	// layout
	QHBoxLayout* hbl = new QHBoxLayout( this );
	hbl->setMargin( 0 );
	hbl->addWidget( lPixmap, 0, Qt::AlignCenter );
	hbl->addWidget( lMessage );
	hbl->addWidget( dbbButtons, 0, Qt::AlignCenter );
	
	// font
	QFont font = this->font();
	font.setPointSize( 9 );
	setFont( font );
	
	// connections
	connect( dbbButtons, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( buttonClicked( QAbstractButton* ) ) );
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

/*!
	\details Return the number of message currently queued
*/
int pQueuedMessageWidget::pendingMessageCount() const
{
	return mMessages.count();
}

pQueuedMessage pQueuedMessageWidget::currentMessage() const
{
	return mMessages.value( 0 );
}

/*!
	\details Append a message to the queued and return it's unique \c id
	\param message The message to show
	\param milliseconds The milliseconds to wait before the message is auto closed, use 0 for unlimited time
*/
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

/*!
	\details Append a message to the queued and return it's unique \c id
	\param message The message structure to add
*/
void pQueuedMessageWidget::append( const pQueuedMessage& message )
{
	if ( !mMessages.contains( message ) ) {
		mMessages << message;
		
		if ( mMessages.count() == 1 ) {
			QTimer::singleShot( 0, this, SLOT( showMessage() ) );
		}
	}
}

/*!
	\details Remove a message from the queue
	\param message The message to remove
*/
void pQueuedMessageWidget::remove( const pQueuedMessage& message )
{
	mMessages.removeOne( message );
}

/*!
	\details Clear the messages
*/
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
	painter.drawRect( rect() );
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

/*!
	\details Show the curernt message.
	\details The widget must be visible as only the gui contents is updated.
*/
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

/*!
	\details Close the current shown message
*/
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
