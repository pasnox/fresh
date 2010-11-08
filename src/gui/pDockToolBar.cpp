#include "pDockToolBar.h"
#include "pDockToolBarManager.h"
#include "pMainWindow.h"
#include "pToolButton.h"

#include <QButtonGroup>
#include <QFrame>
#include <QEvent>
#include <QDockWidget>
#include <QAction>
#include <QKeyEvent>

/*!
	\details Create a new object
	\param manager The pDockToolBarManager manager
	\param orientation The toolbar orientation
	\param window The main window
*/
pDockToolBar::pDockToolBar( pDockToolBarManager* manager, Qt::Orientation orientation )
	: QToolBar( manager->mainWindow() )
{
	mManager = manager;
	mTextAlwaysVisible = true;
	// toggle exclusive action
	aToggleExclusive = new QAction( this );
	aToggleExclusive->setCheckable( true );
	aToggleExclusive->setChecked( true );
	// create button frame
	mFrame = new QFrame( this );
	mFrame->setMinimumSize( QSize( 1, 1 ) );
	// create buttons layout
	mLayout = new QBoxLayout( QBoxLayout::LeftToRight, mFrame );
	mLayout->setMargin( 0 );
	mLayout->setSpacing( 0 );
	// add frame to toolbar
	aDockFrame = addWidget( mFrame );

	// connect orientation change
	connect( this, SIGNAL( orientationChanged( Qt::Orientation ) ), this, SLOT( internal_orientationChanged( Qt::Orientation ) ) );

	// toolbar properties
	setMovable( false );
	setIconSize( QSize( 16, 16 ) );
	setOrientation( orientation );
	
	toggleViewAction()->setEnabled( false );
	toggleViewAction()->setVisible( false );
}

bool pDockToolBar::event( QEvent* event )
{
	if ( event->type() == QEvent::WindowTitleChange ) {
		aToggleExclusive->setText( tr( "%1 exclusive" ).arg( windowTitle() ) );
	}
	
	return QToolBar::event( event );
}

bool pDockToolBar::eventFilter( QObject* object, QEvent* event )
{
	const QEvent::Type type = event->type();
	QDockWidget* dockWidget = qobject_cast<QDockWidget*>( object );
	
	if ( dockWidget ) {
		if ( type == QEvent::KeyPress ) {
			const QKeyEvent* ke = static_cast<QKeyEvent*>( event );
			
			if ( ke->key() == Qt::Key_Escape && ke->modifiers() == Qt::NoModifier ) {
				dockWidget->hide();
			}
		}
	}
	
	return QToolBar::eventFilter( object, event );
}

/*!
	\details Add an action into this pDockToolBar
	\param action The action to add, if null create a new separator action
	\param insert If true the action is inserted before the dock buttons, else it's append
	\return The added action
*/
QAction* pDockToolBar::addAction( QAction* action, bool insert )
{
	if ( !action ) {
		action = new QAction( this );
		action->setSeparator( true );
	}

	if ( insert ) {
		QToolBar::insertAction( aDockFrame, action );
	}
	else {
		QToolBar::addAction( action );
	}

	internal_checkToolBarVisibility();
	
	return action;
}

/*!
	\details Add actions into this pDockToolBar
	\param actions The actions to add
	\param insert If true the actions are inserted before the dock buttons, else they are append
*/
void pDockToolBar::addActions( QList<QAction*> actions, bool insert )
{
	if ( insert ) {
		QToolBar::insertActions( aDockFrame, actions );
	}
	else {
		QToolBar::addActions( actions );
	}

	internal_checkToolBarVisibility();
}

/*!
	\details Add a dock in this toolbar, the dock is moved area if needed
	\param dock The dock to add
	\param title The dock button title
	\param icon The dock button icon
	\return Return the unique id that identify the dock
*/
void pDockToolBar::addDock( QDockWidget* dockWidget, const QString& title, const QIcon& icon )
{
	if ( !dockWidget || mDockWidgets.contains( dockWidget ) ) {
		return;
	}
	
	if ( !title.isEmpty() ) {
		dockWidget->toggleViewAction()->setText( title );
		dockWidget->setWindowTitle( title );
	}
	
	if ( !icon.isNull() ) {
		dockWidget->toggleViewAction()->setIcon( icon );
		dockWidget->setWindowIcon( icon );
	}

	// create button
	const Qt::ToolBarArea tbAreaCurrent = mManager->toolBarArea( this );
	const Qt::DockWidgetArea dwAreaCurrent = mManager->toolBarAreaToDockWidgetArea( tbAreaCurrent );
	const QBoxLayout::Direction blDirection = mManager->toolBarAreaToBoxLayoutDirection( tbAreaCurrent );
	pToolButton* button = addButton( dockWidget, blDirection );
	
	if ( mManager->mainWindow()->dockWidgetArea( dockWidget ) != dwAreaCurrent ) {
		mManager->mainWindow()->addDockWidget( dwAreaCurrent, dockWidget );
	}

	internal_checkButtonExclusivity( dockWidget );
	internal_checkButtonText( button );
	internal_checkToolBarVisibility();
	
	connect( button, SIGNAL( clicked( bool ) ), this, SLOT( internal_buttonClicked( bool ) ) );
}

/*!
	\details Unmanage a dock
	\param dock The dock to unmanage
*/
void pDockToolBar::removeDock( QDockWidget* dockWidget )
{
	if ( !mDockWidgets.contains( dockWidget ) ) {
		return;
	}
	
	dockWidget->removeEventFilter( this );
	delete mDockWidgets.take( dockWidget );
	internal_checkToolBarVisibility();
}

/*!
	\details Return true if the pDockToolBar buttons are exclusive, else false
*/
bool pDockToolBar::exclusive() const
{
	return aToggleExclusive->isChecked();
}

/*!
	\details Set this pDockToolBar exclusive, ie each dock button are exclusive meaning activating one will deactivate all others
	\param exclusive exclusive or not
*/
void pDockToolBar::setExclusive( bool exclusive )
{
	if ( aToggleExclusive->isChecked() == exclusive ) {
		return;
	}
	
	aToggleExclusive->setChecked( exclusive );
	internal_checkButtonExclusivity();
}

/*!
	\details Return true if button text is always shown, else false
*/
bool pDockToolBar::textAlwaysVisible() const
{
	return mTextAlwaysVisible;
}

/*!
	\details If parameter is yes, the text on button will always be shown, else they will be shwn only when they are active
	\param visible Text visibility
*/
void pDockToolBar::setTextAlwaysVisible( bool visible )
{
	mTextAlwaysVisible = visible;
	
	foreach ( pToolButton* button, mDockWidgets ) {
		if ( mTextAlwaysVisible ) {
			if ( button->text().isEmpty() ) {
				button->setText( button->userData().value<QDockWidget*>()->windowTitle() );
			}
		}
		else {
			if ( button->isChecked() && button->text().isEmpty() ) {
				button->setText( button->userData().value<QDockWidget*>()->windowTitle() );
			}
			else if ( !button->isChecked() && !button->text().isEmpty() ) {
				button->setText( QString() );
			}
		}
	}
}

/*!
	\details Set a dock visibility
	\param dock The dock
	\param visible The visible state
*/
void pDockToolBar::setDockVisible( QDockWidget* dockWidget, bool visible )
{
	pToolButton* button = mDockWidgets.value( dockWidget );
	
	if ( button ) {
		if ( dockWidget->isVisible() != visible ) {
			dockWidget->setVisible( visible );
		}
		
		if ( button->isChecked() != visible ) {
			button->setChecked( visible );
		}
		
		internal_checkButtonText( button );
		internal_checkButtonExclusivity( dockWidget );
	}
}

/*!
	\details Return the docks list
*/
QList<QDockWidget*> pDockToolBar::docks( pDockToolBar::DockWidgetsOrder order ) const
{
	QList<QDockWidget*> dockWidgets;
	
	switch ( order ) {
		case pDockToolBar::HashedOrder: {
			dockWidgets = mDockWidgets.keys();
			
			break;
		}
		case pDockToolBar::InsertedOrder: {
			for ( int i = 0; i < mLayout->count(); i++ ) {
				QLayoutItem* item = mLayout->itemAt( i );
				pToolButton* button = qobject_cast<pToolButton*>( item->widget() );
				
				if ( button ) {
					dockWidgets << mDockWidgets.key( button );
				}
			}
			
			break;
		}
	}
	
	return dockWidgets;
}

/*!
	\details Return the dock associated with the button given in parameter
	\param button The button of the dock to get
*/
QDockWidget* pDockToolBar::dock( pToolButton* button ) const
{
	return mDockWidgets.key( button );
}

/*!
	\details Return true if the dockWidget is member of this dockToolBar
	\param dockWidget The dock widget to check
*/
bool pDockToolBar::hasDockWidget( QDockWidget* dockWidget ) const
{
	return mDockWidgets.contains( dockWidget );
}

/*!
	\details Return the buttons list
*/
QList<pToolButton*> pDockToolBar::buttons() const
{
	return mDockWidgets.values();
}

/*!
	\details Return the button associated with the dock given in parameter
	\param dock The dock of the button to get
*/
pToolButton* pDockToolBar::button( QDockWidget* dock ) const
{
	return mDockWidgets.value( dock );
}

/*!
	\details Return the number of docks managed by this pDockToolBar
*/
int pDockToolBar::count() const
{
	return mDockWidgets.count();
}

/*!
	\details Return the togglabe exclusive action associated with this pDockToolBar
*/
QAction* pDockToolBar::toggleExclusiveAction() const 
{
	return aToggleExclusive;
}

void pDockToolBar::setButtonMode( pToolButton* button )
{
	button->setFont( font() );
	
	switch ( mManager->mode() ) {
		case pDockToolBarManager::Classic: {
			const Qt::ToolBarArea tbAreaCurrent = mManager->toolBarArea( this );
			const QBoxLayout::Direction blDirection = mManager->toolBarAreaToBoxLayoutDirection( tbAreaCurrent );
			
			button->setIconSize( iconSize() );
			button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
			button->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed ) );
			button->setDirection( blDirection );
			break;
		}
		case pDockToolBarManager::Modern: {
			button->setIconSize( iconSize() *2 );
			button->setToolButtonStyle( Qt::ToolButtonIconOnly );
			button->setDirection( QBoxLayout::LeftToRight );
			button->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
			break;
		}
		default:
			break;
	}
}

pToolButton* pDockToolBar::addButton( QDockWidget* dockWidget, QBoxLayout::Direction direction )
{
	pToolButton* button = new pToolButton( this, direction );
	
	button->setDefaultAction( dockWidget->toggleViewAction() );
	button->setUserData( QVariant::fromValue( dockWidget ) );
	
	mDockWidgets[ dockWidget ] = button;
	
	dockWidget->installEventFilter( this );
	mLayout->addWidget( button, 0, Qt::AlignCenter );
	
	setButtonMode( button );
	
	return button;
}

void pDockToolBar::internal_checkToolBarVisibility()
{
	// count toolbar actions, if 1 it s dockframe
	int count = actions().count();
	
	if ( !isVisible() && ( count > 1 || ( count == 1 && this->count() ) ) ) {
		show();
	}
	else if ( isVisible() && ( count == 1 && !this->count() ) ) {
		hide();
	}
}

void pDockToolBar::internal_checkButtonExclusivity( QDockWidget* dockWidget )
{
	int visibleDockWidgetsCount = 0;
	
	foreach ( QDockWidget* dw, mDockWidgets.keys() ) {
		if ( dw->isVisible() ) {
			visibleDockWidgetsCount++;
		}
	}
	
	if ( aToggleExclusive->isChecked() && visibleDockWidgetsCount > 1 ) {
		foreach ( QDockWidget* dw, mDockWidgets.keys() ) {
			if ( ( !dockWidget || dw != dockWidget ) && dw->isVisible() ) {
				dw->hide();
				visibleDockWidgetsCount--;
			}
			
			if ( visibleDockWidgetsCount == 1 ) {
				break;
			}
		}
	}
}

void pDockToolBar::internal_checkButtonText( pToolButton* button )
{
	if ( !button ) {
		return;
	}
	
	// show text when checked, else not
	if ( !mTextAlwaysVisible ) {
		QDockWidget* dockWidget = button->userData().value<QDockWidget*>();
		
		if ( button->isChecked() && button->text().isEmpty() ) {
			button->setText( dockWidget->windowTitle() );
		}
		else if ( !button->isChecked() && !button->text().isEmpty() ) {
			button->setText( QString() );
		}
	}
}

void pDockToolBar::internal_updateButtonsState()
{
	foreach ( pToolButton* bt, findChildren<pToolButton*>() ) {
		setButtonMode( bt );
	}
}

void pDockToolBar::internal_orientationChanged( Qt::Orientation orientation )
{
	switch ( orientation ) {
		case Qt::Horizontal:
			mLayout->setDirection( QBoxLayout::LeftToRight );
			break;
		case Qt::Vertical:
			mLayout->setDirection( QBoxLayout::TopToBottom );
			break;
	}
	
	internal_updateButtonsState();
}

void pDockToolBar::internal_buttonClicked( bool checked )
{
	pToolButton* button = qobject_cast<pToolButton*>( sender() );
	QDockWidget* dockWidget = this->dock( button );

	if ( !dockWidget ) {
		return;
	}
	
	setDockVisible( dockWidget, checked );
}
