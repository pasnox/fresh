#include "pDockWidget.h"
#include "pDockWidgetTitleBar.h"

#include <QPainter>
#include <QStyle>

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
