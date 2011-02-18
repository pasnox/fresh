#include "pDockWidgetTitleBar.h"
#include "pToolButton.h"
#include "pGuiUtils.h"

#include <QAction>
#include <QStyleOptionToolButton>
#include <QMenu>
#include <QEvent>
#include <QPainter>

pDockWidgetTitleBar::pDockWidgetTitleBar( QDockWidget* parent )
	: QToolBar( parent )
{
	Q_ASSERT( parent );
	mDock = parent;
	
	// a fake spacer widget
	QWidget* spacer = new QWidget( this );
	spacer->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
	
	tbOrientation = new pToolButton( this );
	tbOrientation->installEventFilter( this );
	tbFloat = new pToolButton( this );
	tbFloat->installEventFilter( this );
	tbClose = new pToolButton( this );
	tbClose->installEventFilter( this );
	
	addWidget( spacer );
	aOrientation = addWidget( tbOrientation );
	aFloat = addWidget( tbFloat );
	aClose = addWidget( tbClose );
	
	tbOrientation->setDefaultAction( aOrientation );
	tbFloat->setDefaultAction( aFloat );
	tbClose->setDefaultAction( aClose );
	
	setMovable( false );
	setFloatable( false );
	
	updateStyleChange();
	dockWidget_featuresChanged( mDock->features() );
	
	connect( mDock, SIGNAL( featuresChanged( QDockWidget::DockWidgetFeatures ) ), this, SLOT( dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures ) ) );
	connect( aOrientation, SIGNAL( triggered() ), this, SLOT( aOrientation_triggered() ) );
	connect( aFloat, SIGNAL( triggered() ), this, SLOT( aFloat_triggered() ) );
	connect( aClose, SIGNAL( triggered() ), mDock, SLOT( close() ) );
}

QIcon pDockWidgetTitleBar::icon() const
{
	QIcon icon = mDock->toggleViewAction()->icon();
	
	if ( icon.isNull() ) {
		icon = mDock->windowIcon();
	}
	
	if ( icon.isNull() ) {
		icon =  toggleViewAction()->icon();
	}
	
	if ( icon.isNull() ) {
		icon =  windowIcon();
	}
	
	if ( icon.isNull() ) {
		icon = window()->windowIcon();
	}
	
	return icon;
}

QSize pDockWidgetTitleBar::windowIconSize() const
{
	const int size = orientation() == Qt::Horizontal ? height() -2 : width() -2;
	return icon().isNull() ? QSize() : QSize( size, size );
}

bool pDockWidgetTitleBar::eventFilter( QObject* object, QEvent* event )
{
	pToolButton* button = qobject_cast<pToolButton*>( object );
	
	if ( button && event->type() == QEvent::Paint ) {
		QStyleOptionToolButton option;
		button->initStyleOption( &option );
		option.icon = QIcon();
		
		button->paint( &option );
		QPainter painter( button );
		button->icon().paint( &painter, button->rect(), Qt::AlignCenter, QIcon::Normal, QIcon::Off );
		
		event->accept();
		return true;
	}
	
	return QToolBar::eventFilter( object, event );
}

void pDockWidgetTitleBar::paintEvent( QPaintEvent* event )
{
	Q_UNUSED( event );
	
	QRect rect = this->rect();
	QTransform transform;
	QPainter painter( this );
	
	if ( mDock->features() & QDockWidget::DockWidgetVerticalTitleBar ) {
		rect.setSize( QSize( rect.height(), rect.width() ) );
		transform.rotate( -90 );
		transform.translate( -rect.width() +1, 0 );
	}
	
	painter.setTransform( transform );
	
	// draw background
	QStyleOptionToolBar optionTb;
	initStyleOption( &optionTb );
	optionTb.rect = rect;
	
	style()->drawControl( QStyle::CE_ToolBar, &optionTb, &painter, window() );
	
	// icon / title
	QStyleOptionButton optionB;
	optionB.initFrom( mDock );
	optionB.rect = rect.adjusted( 2, 0, -( orientation() == Qt::Horizontal ? minimumSizeHint().width() : minimumSizeHint().height() ), 0 );
	optionB.text = mDock->windowTitle();
	optionB.iconSize = windowIconSize();
	optionB.icon = icon();
	
	style()->drawControl( QStyle::CE_PushButtonLabel, &optionB, &painter, mDock );
}

void pDockWidgetTitleBar::updateStyleChange()
{
	setIconSize( QSize( 13, 13 ) );
	layout()->setSpacing( 0 );
	layout()->setMargin( 2 );
	
	QIcon icon;
	
	//icon = pGuiUtils::scaledPixmap( orientation() == Qt::Horizontal ? ":/fresh/icons/vertical.png" : ":/fresh/icons/horizontal.png", iconSize() );
	icon = style()->standardIcon( QStyle::SP_TitleBarShadeButton, 0, widgetForAction( aOrientation ) );
	aOrientation->setIcon( icon );
	
	icon = style()->standardIcon( QStyle::SP_TitleBarNormalButton, 0, widgetForAction( aFloat ) );
	aFloat->setIcon( icon );
	
	icon = style()->standardIcon( QStyle::SP_TitleBarCloseButton, 0, widgetForAction( aClose ) );
	aClose->setIcon( icon );
	
	if ( orientation() == Qt::Horizontal ) {
		tbOrientation->setDirection( QBoxLayout::LeftToRight );
		tbFloat->setDirection( QBoxLayout::LeftToRight );
		tbClose->setDirection( QBoxLayout::LeftToRight );
	}
	else {
		tbOrientation->setDirection( QBoxLayout::BottomToTop );
		tbFloat->setDirection( QBoxLayout::BottomToTop );
		tbClose->setDirection( QBoxLayout::BottomToTop );
	}
}

bool pDockWidgetTitleBar::event( QEvent* event )
{
	if ( event->type() == QEvent::StyleChange ) {
		updateStyleChange();
	}
	
	return QToolBar::event( event );
}

QSize pDockWidgetTitleBar::minimumSizeHint() const
{
	return QToolBar::sizeHint();
}

QSize pDockWidgetTitleBar::sizeHint() const
{
	const QSize wis = windowIconSize();
	QSize size = QToolBar::sizeHint();
	QFontMetrics fm( font() );

	if ( mDock->features() & QDockWidget::DockWidgetVerticalTitleBar ) {
		size.rheight() += fm.width( mDock->windowTitle() ) +wis.width();
	}
	else {
		size.rwidth() += fm.width( mDock->windowTitle() ) +wis.width();
	}

	return size;
}

QWidget* pDockWidgetTitleBar::addAction( QAction* action, int index )
{
	if ( index != -1 ) {
		index++;
	}
	
	if ( index >= 0 && index < actions().count() ) {
		QToolBar::insertAction( actions().value( index ), action );
	}
	else {
		QToolBar::addAction( action );
	}
	
	return widgetForAction( action );
}

void pDockWidgetTitleBar::addSeparator( int index )
{
	if ( index != -1 ) {
		index++;
	}
	
	if ( index >= 0 && index < actions().count() ) {
		QToolBar::insertSeparator( actions().value( index ) );
	}
	else {
		QToolBar::addSeparator();
	}
}

void pDockWidgetTitleBar::aOrientation_triggered()
{
	const QDockWidget::DockWidgetFeatures features = mDock->features();
	
	if ( features & QDockWidget::DockWidgetVerticalTitleBar ) {
		mDock->setFeatures( features ^ QDockWidget::DockWidgetVerticalTitleBar );
	}
	else {
		mDock->setFeatures( features | QDockWidget::DockWidgetVerticalTitleBar );
	}
}

void pDockWidgetTitleBar::aFloat_triggered()
{
	mDock->setFloating( !mDock->isFloating() );
}

void pDockWidgetTitleBar::dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures features )
{
	aFloat->setVisible( features & QDockWidget::DockWidgetFloatable );
	aClose->setVisible( features & QDockWidget::DockWidgetClosable );
	
	// update toolbar orientation
	if ( features & QDockWidget::DockWidgetVerticalTitleBar ) {
		if ( orientation() == Qt::Vertical ) {
			return;
		}
		
		setOrientation( Qt::Vertical );
	}
	else {
		if ( orientation() == Qt::Horizontal ) {
			return;
		}
		
		setOrientation( Qt::Horizontal );
	}
	
	// re-order the actions
	const QList<QAction*> actions = this->actions();
	QList<QAction*> items;
	
	for ( int i = actions.count() -1; i > -1; i-- ) {
		items << actions.at( i );
	}
	
	clear();
	addActions( items );
	updateStyleChange();
}
