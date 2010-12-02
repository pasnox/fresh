#include "pQueuedMessageToolBar.h"

#include <QAction>
#include <QPainter>
#include <QLayout>
#include <QDebug>

/*!
	\details Create a new pQueuedMessageToolBar object
	\param parent The parent widget
*/
pQueuedMessageToolBar::pQueuedMessageToolBar( QWidget* parent )
	: QToolBar( parent )
{
	mQueuedWidget = new pQueuedMessageWidget( this );
	
	setObjectName( metaObject()->className() );
	setMovable( false );
	setFloatable( false );
	setAllowedAreas( Qt::TopToolBarArea );
	toggleViewAction()->setEnabled( false );
	toggleViewAction()->setVisible( false );
	
	layout()->setMargin( 3 );
	addWidget( mQueuedWidget );
	
	// connections
	connect( mQueuedWidget, SIGNAL( shown( const pQueuedMessage& ) ), this, SLOT( messageShown( const pQueuedMessage& ) ) );
	connect( mQueuedWidget, SIGNAL( finished() ), this, SLOT( messageFinished() ) );
}

pQueuedMessageToolBar::~pQueuedMessageToolBar()
{
}

pQueuedMessageWidget* pQueuedMessageToolBar::queuedMessageWidget() const
{
	return mQueuedWidget;
}

void pQueuedMessageToolBar::paintEvent( QPaintEvent* event )
{
	if ( mQueuedWidget->pendingMessageCount() == 0 ) {
		QToolBar::paintEvent( event );
		return;
	}
	
	const QBrush brush = mQueuedWidget->currentMessageBackground();
	QPainter painter( this );
	painter.setPen( brush.color().darker( 150 ) );
	painter.setBrush( brush );
	painter.drawRect( rect().adjusted( 0, 0, -1, -1 ) );
}

/*!
	\details Append a new message and return it's id
	\param message The message to show
	\param milliseconds The timeout before the message is auto closed, 0 for no timeout
*/
pQueuedMessage pQueuedMessageToolBar::appendMessage( const QString& message, int milliSeconds )
{
	return mQueuedWidget->append( message, milliSeconds );
}

/*!
	\details Append a new message and return it's id
	\param message The message structure to show
*/
void pQueuedMessageToolBar::appendMessage( const pQueuedMessage& message )
{
	mQueuedWidget->append( message );
}

/*!
	\details Remove a message from the queued list
	\param message The message structure to remove
*/
void pQueuedMessageToolBar::removeMessage( const pQueuedMessage& message )
{
	mQueuedWidget->remove( message );
}

void pQueuedMessageToolBar::messageShown( const pQueuedMessage& message )
{
	Q_UNUSED( message );
	
	if ( !isVisible() )
	{
		setVisible( true );
	}
}

void pQueuedMessageToolBar::messageFinished()
{
	if ( isVisible() )
	{
		setVisible( false );
	}
}
