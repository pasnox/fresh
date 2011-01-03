#include "pDockWidget.h"
#include "pDockWidgetTitleBar.h"

#include <QPainter>
#include <QStyle>
#include <QTimer>
#include <QAction>

/*!
	\brief Create a new pDockWidget instance
	\param title The dock title
	\param parent The dock parent
	\param flags The dock window flags
*/
pDockWidget::pDockWidget( const QString& title, QWidget* parent, Qt::WindowFlags flags )
	: QDockWidget( title, parent, flags )
{
	init();
}

/*!
	\brief Create a new pDockWidget instance
	\param parent The dock parent
	\param flags The dock window flags
*/
pDockWidget::pDockWidget( QWidget* parent, Qt::WindowFlags flags )
	: QDockWidget( parent, flags )
{
	init();
}

void pDockWidget::init()
{
	mTitleBar = new pDockWidgetTitleBar( this );
	setTitleBarWidget( mTitleBar );
	connect( toggleViewAction(), SIGNAL( toggled( bool ) ), this, SLOT( toggleViewAction_toggled( bool ) ) );
}

void pDockWidget::paintEvent( QPaintEvent* event )
{
	QDockWidget::paintEvent(  event );
	
	if ( isFloating() && style()->objectName().compare( "Oxygen", Qt::CaseInsensitive ) != 0 ) {
		QRect rect = this->rect().adjusted( 0, 0, -1, -1 );
		
		QPainter painter( this );
		painter.setPen( QColor( 145, 142, 142 ) );
		painter.setBrush( Qt::NoBrush );
		painter.drawRect( rect );
	}
}

pDockWidgetTitleBar* pDockWidget::titleBar() const
{
	return mTitleBar;
}

void pDockWidget::toggleViewAction_toggled( bool toggled )
{
	if ( toggled && focusProxy() )
	{
		if ( isFloating() )
		{
			QTimer::singleShot( 0, this, SLOT( handleWindowActivation() ) );
		}
		else
		{
			QTimer::singleShot( 0, this, SLOT( handleFocusProxy() ) );
		}
	}
}

void pDockWidget::handleWindowActivation()
{
	activateWindow();
	QTimer::singleShot( 0, this, SLOT( handleFocusProxy() ) );
}

void pDockWidget::handleFocusProxy()
{
	focusProxy()->setFocus();
}
