#include "pDockWidgetTitleBar.h"
#include "gui/pToolButton.h"
#include "gui/pDrawingUtils.h"

#include <QAction>
#include <QStyleOptionToolButton>
#include <QMenu>
#include <QEvent>
#include <QPainter>

pDockWidgetTitleBar::pDockWidgetTitleBar( pDockWidget* parent )
	: QToolBar( parent )
{
	Q_ASSERT( parent );
	mDock = parent;
	
	// a fake spacer widget
	QWidget* widget = new QWidget( this );
	widget->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
	
	aOrientation = new QAction( this );
	aFloat = new QAction( this );
	aClose = new QAction( this );
	
	tbOrientation = new pToolButton( this );
	tbOrientation->setDefaultAction( aOrientation );
	tbOrientation->installEventFilter( this );
	tbFloat = new pToolButton( this );
	tbFloat->setDefaultAction( aFloat );
	tbFloat->installEventFilter( this );
	tbClose = new pToolButton( this );
	tbClose->setDefaultAction( aClose );
	tbClose->installEventFilter( this );
	
	addWidget( widget );
	addWidget( tbOrientation );
	addWidget( tbFloat );
	addWidget( tbClose );
	
	setMovable( false );
	setFloatable( false );
	
	updateStyleChange();
	dockWidget_featuresChanged( mDock->features() );
	
	connect( mDock, SIGNAL( featuresChanged( QDockWidget::DockWidgetFeatures ) ), this, SLOT( dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures ) ) );
	connect( aOrientation, SIGNAL( triggered() ), this, SLOT( aOrientation_triggered() ) );
	connect( aFloat, SIGNAL( triggered() ), this, SLOT( aFloat_triggered() ) );
	connect( aClose, SIGNAL( triggered() ), mDock, SLOT( close() ) );
}

QSize pDockWidgetTitleBar::windowIconSize() const
{
	const int size = orientation() == Qt::Horizontal ? height() -2 : width() -2;
	return mDock->windowIcon().isNull() ? QSize() : QSize( size, size );
}

/*!
    Initialize \a option with the values from this QToolButton. This method
    is useful for subclasses when they need a QStyleOptionToolButton, but don't want
    to fill in all the information themselves.

    \sa QStyleOption::initFrom()
*/
void pDockWidgetTitleBar::initStyleOptionToolButton( QToolButton* button, QStyleOptionToolButton* option ) const
{
    if ( !button || !option ) {
        return;
	}
	
    option->initFrom( button );
    bool forceNoText = false;
    option->iconSize = button->iconSize();
	
    if ( button->parentWidget() ) {
        if ( QToolBar* toolBar = qobject_cast<QToolBar*>( button->parentWidget() ) ) {
            option->iconSize = toolBar->iconSize();
        }
    }
	
    if ( !forceNoText ) {
        option->text = button->text();
	}
	
    option->icon = button->icon();
    option->arrowType = button->arrowType();
	
    if ( button->isDown() ) {
        option->state |= QStyle::State_Sunken;
	}
	
    if ( button->isChecked() ) {
        option->state |= QStyle::State_On;
	}
	
    if ( button->autoRaise() ) {
        option->state |= QStyle::State_AutoRaise;
	}
	
    if ( !button->isChecked() && !button->isDown() ) {
        option->state |= QStyle::State_Raised;
	}

    option->subControls = QStyle::SC_ToolButton;
    option->activeSubControls = QStyle::SC_None;

    option->features = QStyleOptionToolButton::None;
	
    if ( button->popupMode() == QToolButton::MenuButtonPopup ) {
        option->subControls |= QStyle::SC_ToolButtonMenu;
        option->features |= QStyleOptionToolButton::MenuButtonPopup;
    }
	
    if ( option->state & QStyle::State_MouseOver ) {
		QStyle::SubControls subControls = option->subControls;
		option->subControls = QStyle::SC_All;
		QStyle::SubControl hoverControl = button->style()->hitTestComplexControl( QStyle::CC_ToolButton, option, button->mapFromGlobal( QCursor::pos() ), button );
        option->activeSubControls = hoverControl; // was d->hoverControl();
		option->subControls = subControls;
    }
	
    if ( button->menu() && button->menu()->isVisible() ) { // was d->menuButtonDown
        option->state |= QStyle::State_Sunken;
        option->activeSubControls |= QStyle::SC_ToolButtonMenu;
    }
	
    if ( button->isDown() ) {
        option->state |= QStyle::State_Sunken;
        option->activeSubControls |= QStyle::SC_ToolButton;
    }
	
    if ( button->arrowType() != Qt::NoArrow ) {
        option->features |= QStyleOptionToolButton::Arrow;
	}
	
    if ( button->popupMode() == QToolButton::DelayedPopup ) {
        option->features |= QStyleOptionToolButton::PopupDelay;
	}
	
#ifndef QT_NO_MENU
    if ( button->menu() ) {
        option->features |= QStyleOptionToolButton::HasMenu;
	}
#endif
	
    if ( button->toolButtonStyle() == Qt::ToolButtonFollowStyle ) {
        option->toolButtonStyle = Qt::ToolButtonStyle( style()->styleHint( QStyle::SH_ToolButtonStyle, option, button ) );
    }
	else {
        option->toolButtonStyle = button->toolButtonStyle();
	}
	
    if ( option->toolButtonStyle == Qt::ToolButtonTextBesideIcon ) {
        // If the action is not prioritized, remove the text label to save space
        if ( button->defaultAction() && button->defaultAction()->priority() < QAction::NormalPriority ) {
            option->toolButtonStyle = Qt::ToolButtonIconOnly;
		}
    }
	
    if ( button->icon().isNull() && button->arrowType() == Qt::NoArrow && !forceNoText ) {
        if ( !button->text().isEmpty() ) {
            option->toolButtonStyle = Qt::ToolButtonTextOnly;
		}
        else if ( option->toolButtonStyle != Qt::ToolButtonTextOnly ) {
            option->toolButtonStyle = Qt::ToolButtonIconOnly;
		}
    }
	else {
        if ( button->text().isEmpty() && option->toolButtonStyle != Qt::ToolButtonIconOnly ) {
            option->toolButtonStyle = Qt::ToolButtonIconOnly;
		}
    }
	
    option->pos = button->pos();
    option->font = button->font();
}

bool pDockWidgetTitleBar::eventFilter( QObject* object, QEvent* event )
{
	pToolButton* button = qobject_cast<pToolButton*>( object );
	
	if ( button && event->type() == QEvent::Paint ) {
		QPainter painter( button );
		QTransform transform;
		
		QStyleOptionToolButton option;
		initStyleOptionToolButton( button, &option );
		option.state &= ~QStyle::State_On;
		option.icon = QIcon();
		
		switch ( button->direction() ) {
			case QBoxLayout::TopToBottom:
				option.rect.setSize( button->internalSize( Qt::Horizontal ) );
				transform.rotate( 90 );
				transform.translate( 0, -option.rect.height() +1 );
				break;
			case QBoxLayout::BottomToTop:
				option.rect.setSize( button->internalSize( Qt::Horizontal ) );
				transform.rotate( -90 );
				transform.translate( -option.rect.width() +1, 0 );
				break;
			default:
				break;
		}
		
		painter.setTransform( transform );
		
		button->style()->drawComplexControl( QStyle::CC_ToolButton, &option, &painter, mDock );
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
	optionB.icon = mDock->windowIcon();
	
	style()->drawControl( QStyle::CE_PushButtonLabel, &optionB, &painter, mDock );
}

void pDockWidgetTitleBar::updateStyleChange()
{
	setIconSize( QSize( 13, 13 ) );
	layout()->setSpacing( 0 );
	layout()->setMargin( 2 );
	
	QIcon icon;
	
	icon = pDrawingUtils::scaledPixmap( orientation() == Qt::Horizontal ? ":/fresh/icons/vertical.png" : ":/fresh/icons/horizontal.png", iconSize() );
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
	QList<QAction*> items;
	
	for ( int i = actions().count() -1; i > -1; i-- ) {
		items << actions().at( i );
	}
	
	clear();
	addActions( items );
	updateStyleChange();
}
