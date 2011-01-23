#include "pQueuedMessageToolBar.h"

#include <QAction>
#include <QPainter>
#include <QLayout>
#include <QDebug>

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

pQueuedMessage pQueuedMessageToolBar::appendMessage( const QString& message, int milliSeconds )
{
	return mQueuedWidget->append( message, milliSeconds );
}

void pQueuedMessageToolBar::appendMessage( const pQueuedMessage& message )
{
	mQueuedWidget->append( message );
}

void pQueuedMessageToolBar::removeMessage( const pQueuedMessage& message )
{
	mQueuedWidget->remove( message );
}

void pQueuedMessageToolBar::messageShown( const pQueuedMessage& message )
{
	Q_UNUSED( message );
	
	if ( !isVisible() ) {
		setVisible( true );
	}
}

void pQueuedMessageToolBar::messageFinished()
{
	if ( isVisible() ) {
		setVisible( false );
	}
}
