#include "pMainWindow.h"
#include "core/pSettings.h"
#include "actionmanager/pActionsNodeMenuBar.h"
#include "queuedmessage/pQueuedMessageToolBar.h"
#include "dockmanager/pDockToolBarManager.h"
#include "dockmanager/pDockToolBar.h"

/*!
	\details Create a new pMainWindow object
	\param parent The parent widget
	\param windowFlags The window flags
*/
pMainWindow::pMainWindow( QWidget* parent, Qt::WindowFlags windowFlags )
	: QMainWindow( parent, windowFlags )
{
	mShown = false;
	mSettings = 0;
	mMenuBar = 0;
	mQueuedMessageToolBar = 0;
	mDockToolBarManager = 0;
	
#if not defined( Q_OS_MAC )
	setMenuBar( menuBar() );
#else
	menuBar();
#endif

	queuedMessageToolBar();
}

pMainWindow::~pMainWindow()
{
	if ( settings() ) {
		settings()->sync();
	}
	
#if defined( Q_OS_MAC )
	delete mMenuBar;
#endif
}

QMenu* pMainWindow::createPopupMenu()
{
	QMenu* menu = QMainWindow::createPopupMenu();
	
	menu->addSeparator();
	menu->addAction( dockToolBarManager()->dockToolBar( Qt::TopToolBarArea )->toggleExclusiveAction() );
	menu->addAction( dockToolBarManager()->dockToolBar( Qt::RightToolBarArea )->toggleExclusiveAction() );
	menu->addAction( dockToolBarManager()->dockToolBar( Qt::BottomToolBarArea )->toggleExclusiveAction() );
	menu->addAction( dockToolBarManager()->dockToolBar( Qt::LeftToolBarArea )->toggleExclusiveAction() );
	
	return menu;
}

void pMainWindow::showEvent( QShowEvent* event )
{
	QMainWindow::showEvent( event );
	
	if ( !mShown ) {
		mShown = true;
		restoreState();
	}
}

void pMainWindow::closeEvent( QCloseEvent* event )
{
	saveState();
	QMainWindow::closeEvent( event );
}

/*!
	\details Return the pSettings object
*/
pSettings* pMainWindow::settings() const
{
	if ( !mSettings ) {
		mSettings = new pSettings( const_cast<pMainWindow*>( this ) );
	}
	
	return mSettings;
}

/*!
	\details Return the pMenuBar object
*/
pActionsNodeMenuBar* pMainWindow::menuBar() const
{
	if ( !mMenuBar ) {
#if defined( Q_OS_MAC )
		mMenuBar = new pActionsNodeMenuBar;
#else
		mMenuBar = new pActionsNodeMenuBar( const_cast<pMainWindow*>( this ) );
#endif
	}
	
	return mMenuBar;
}

pQueuedMessageToolBar* pMainWindow::queuedMessageToolBar() const
{
	if ( !mQueuedMessageToolBar ) {
		pMainWindow* mw = const_cast<pMainWindow*>( this );
		mQueuedMessageToolBar = new pQueuedMessageToolBar( mw );
		mw->addToolBar( Qt::TopToolBarArea, mQueuedMessageToolBar );
		mQueuedMessageToolBar->setVisible( false );
	}
	
	return mQueuedMessageToolBar;
}

/*!
	\details Return the pDockToolBarManager object
*/
pDockToolBarManager* pMainWindow::dockToolBarManager() const
{
	if ( !mDockToolBarManager ) {
		mDockToolBarManager = new pDockToolBarManager( const_cast<pMainWindow*>( this ) );
	}
	
	return mDockToolBarManager;
}

/*!
	\details Return the pDockToolBar object for \c area
	\param area The area of the bar to get
*/
pDockToolBar* pMainWindow::dockToolBar( Qt::ToolBarArea area ) const
{
	return dockToolBarManager()->dockToolBar( area );
}

/*!
	\details Save the pMainWindow state.
	\details Concretly pDockToolBarManager::saveState() and
	\details pSettings::saveState() are called.
*/
void pMainWindow::saveState()
{
	if ( settings() ) {
		dockToolBarManager()->saveState();
		settings()->saveState( this );
	}
}

/*!
	\details Restore the pMainWindow state.
	\details Concretly pDockToolBarManager::restoreState() and
	\details pSettings::restoreState() are called.
*/
void pMainWindow::restoreState()
{
	if ( settings() ) {
		dockToolBarManager()->setRestoring( true );
		settings()->restoreState( this );
		queuedMessageToolBar()->setVisible( false );
		dockToolBarManager()->setRestoring( false );
		dockToolBarManager()->restoreState();
	}
}
