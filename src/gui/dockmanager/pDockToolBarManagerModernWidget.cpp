#include "pDockToolBarManagerModernWidget.h"

#include "pDockToolBar.h"
#include "pToolButton.h"

#include <QScrollArea>
#include <QSplitter>
#include <QGroupBox>
#include <QBoxLayout>
#include <QScrollBar>
#include <QAction>

pDockToolBarManagerModernWidget::pDockToolBarManagerModernWidget( QWidget* parent )
	: QToolBar( parent )
{
	setObjectName( "pDockToolBarManagerModernWidget" );
	setWindowTitle( tr( "Modern dock toolbar manager" ) );
	
	toggleViewAction()->setEnabled( false );
	toggleViewAction()->setVisible( false );
	
	mContent = new QWidget( this );
	mContent->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );
	mLayout = new QBoxLayout( QBoxLayout::TopToBottom, mContent );
	mLayout->setMargin( 0 );
	mLayout->setSpacing( 0 );
	mLayout->addStretch();
	
	mArea = new QScrollArea( this );
	mArea->setWidgetResizable( true );
	mArea->setFrameStyle( QFrame::NoFrame | QFrame::Plain );
	mArea->setMinimumSize( QSize( 1, 1 ) );
	mArea->setWidget( mContent );
	
	QPalette pal = mArea->palette();
	pal.setColor( QPalette::Window, QColor( Qt::transparent ) );
	mArea->setPalette( pal );
	
	addWidget( mArea );
	
	layout()->setMargin( 3 );
	layout()->setSpacing( 0 );
	
	connect( this, SIGNAL( orientationChanged( Qt::Orientation ) ), this, SLOT( _q_orientationChanged( Qt::Orientation ) ) );
}

pDockToolBarManagerModernWidget::~pDockToolBarManagerModernWidget()
{
}

QSize pDockToolBarManagerModernWidget::sizeHint() const
{
	const int margin = layout()->margin() *2;
	const QSize sbSize(
		mArea->verticalScrollBar()->isVisible() ? mArea->verticalScrollBar()->width() +2 : 0
		,
		mArea->horizontalScrollBar()->isVisible() ? mArea->horizontalScrollBar()->height() +2 : 0
	);
	return QSize( isFloating()
		? ( orientation() == Qt::Horizontal ? QSize( parentWidget()->width(), 60 ) : QSize( 60, parentWidget()->height() ) )
		: mContent->sizeHint() +sbSize ) +QSize( margin, margin );
}

void pDockToolBarManagerModernWidget::setToolBars( const QList<pDockToolBar*>& toolBars )
{
	foreach ( pDockToolBar* tb, toolBars ) {
		mLayout->insertWidget( mLayout->count() -1, tb );
	}
	
	_q_orientationChanged( orientation() );
}

void pDockToolBarManagerModernWidget::_q_orientationChanged( Qt::Orientation orientation )
{
	mLayout->setDirection( orientation == Qt::Horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom );
	
	foreach ( pDockToolBar* tb, mContent->findChildren<pDockToolBar*>() ) {
		tb->setOrientation( orientation );
		
		switch ( orientation ) {
			case Qt::Horizontal:
				tb->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );
				break;
			case Qt::Vertical:
				tb->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
				break;
		}
	}
}
