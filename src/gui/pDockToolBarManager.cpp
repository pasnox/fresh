#include "pDockToolBarManager.h"
#include "pMainWindow.h"
#include "pDockToolBar.h"
#include "core/pSettings.h"

#include <QDockWidget>
#include <QAbstractButton>
#include <QAction>
#include <QChildEvent>

/*!
	\details Create a new pDockToolBarManager
	\param window The QMainWindow where this manager operate
*/
pDockToolBarManager::pDockToolBarManager( pMainWindow* window )
	: QObject( window )
{
	Q_ASSERT( window );
	mMainWindow = window;
	mIsRestoring = false;
	initializeToolBars();
	mMainWindow->installEventFilter( this );
}

bool pDockToolBarManager::eventFilter( QObject* object, QEvent* event )
{
	switch ( event->type() ) {
		case QEvent::ChildPolished:
		{
			QChildEvent* ce = static_cast<QChildEvent*>( event );
			QDockWidget* dock = qobject_cast<QDockWidget*>( ce->child() );
			
			if ( dock ) {
				trackDockWidget( dock );
			}
			
			break;
		}
		case QEvent::ChildRemoved:
		{
			QChildEvent* ce = static_cast<QChildEvent*>( event );
			QDockWidget* dock = qobject_cast<QDockWidget*>( ce->child() );
			
			if ( dock ) {
				untrackDockWidget( dock );
			}
			
			break;
		}
		default:
			break;
	}
	
	return QObject::eventFilter( object, event );
}

bool pDockToolBarManager::isRestoring() const
{
	return mIsRestoring;
}

void pDockToolBarManager::setRestoring( bool restoring )
{
	mIsRestoring = restoring;
}

/*!
	\details Return the associated pMainWindow
*/
pMainWindow* pDockToolBarManager::mainWindow() const
{
	return mMainWindow;
}

/*!
	\details Return the pDockToolBars of the manager.
*/
QList<pDockToolBar*> pDockToolBarManager::dockToolBars() const
{
	return mDockToolBars.values();
}

/*!
	\details Return a pDockToolBar for the area, creating it if needed.
	\details Return null if area is invalid
	\param area The area to get/create the pDockToolBar
*/
pDockToolBar* pDockToolBarManager::dockToolBar( Qt::ToolBarArea area ) const
{
	return mDockToolBars[ area ];
}

pDockToolBar* pDockToolBarManager::dockToolBar( Qt::DockWidgetArea area ) const
{
	return dockToolBar( dockWidgetAreaToToolBarArea( area ) );
}

pDockToolBar* pDockToolBarManager::dockToolBar( QDockWidget* dockWidget ) const
{
	foreach ( pDockToolBar* dockToolBar, mDockToolBars ) {
		if ( dockToolBar->docks().contains( dockWidget ) ) {
			return dockToolBar;
		}
	}
	
	return 0;
}

void pDockToolBarManager::initializeToolBars()
{
	// Qt::TopToolBarArea
	mDockToolBars[ Qt::TopToolBarArea ] = new pDockToolBar( this, Qt::Horizontal );
	mDockToolBars[ Qt::TopToolBarArea ]->setObjectName( "pDockToolBarTop" );
	mDockToolBars[ Qt::TopToolBarArea ]->setWindowTitle( tr( "Top toolbar" ) );
	mDockToolBars[ Qt::TopToolBarArea ]->toggleViewAction()->setObjectName( "pDockToolBarTopViewAction" );
	mDockToolBars[ Qt::TopToolBarArea ]->toggleViewAction()->setText( tr( "Top toolbar visible" ) );
	mDockToolBars[ Qt::TopToolBarArea ]->toggleExclusiveAction()->setObjectName( "pDockToolBarTopExclusiveAction" );
	mMainWindow->addToolBar( Qt::TopToolBarArea, mDockToolBars[ Qt::TopToolBarArea ] );
	// Qt::BottomToolBarArea
	mDockToolBars[ Qt::BottomToolBarArea ] = new pDockToolBar( this, Qt::Horizontal );
	mDockToolBars[ Qt::BottomToolBarArea ]->setObjectName( "pDockToolBarBottom" );
	mDockToolBars[ Qt::BottomToolBarArea ]->setWindowTitle( tr( "Bottom toolbar" ) );
	mDockToolBars[ Qt::BottomToolBarArea ]->toggleViewAction()->setObjectName( "pDockToolBarBottomViewAction" );
	mDockToolBars[ Qt::BottomToolBarArea ]->toggleViewAction()->setText( tr( "Bottom toolbar visible" ) );
	mDockToolBars[ Qt::BottomToolBarArea ]->toggleExclusiveAction()->setObjectName( "pDockToolBarBottomExclusiveAction" );
	mMainWindow->addToolBar( Qt::BottomToolBarArea, mDockToolBars[ Qt::BottomToolBarArea ] );
	// Qt::LeftToolBarArea
	mDockToolBars[ Qt::LeftToolBarArea ] = new pDockToolBar( this, Qt::Vertical );
	mDockToolBars[ Qt::LeftToolBarArea ]->setObjectName( "pDockToolBarLeft" );
	mDockToolBars[ Qt::LeftToolBarArea ]->setWindowTitle( tr( "Left toolbar" ) );
	mDockToolBars[ Qt::LeftToolBarArea ]->toggleViewAction()->setObjectName( "pDockToolBarLeftViewAction" );
	mDockToolBars[ Qt::LeftToolBarArea ]->toggleViewAction()->setText( tr( "Left toolbar visible" ) );
	mDockToolBars[ Qt::LeftToolBarArea ]->toggleExclusiveAction()->setObjectName( "pDockToolBarLeftExclusiveAction" );
	mMainWindow->addToolBar( Qt::LeftToolBarArea, mDockToolBars[ Qt::LeftToolBarArea ] );
	// Qt::RightToolBarArea
	mDockToolBars[ Qt::RightToolBarArea ] = new pDockToolBar( this, Qt::Vertical );
	mDockToolBars[ Qt::RightToolBarArea ]->setObjectName( "pDockToolBarRight" );
	mDockToolBars[ Qt::RightToolBarArea ]->setWindowTitle( tr( "Right toolbar" ) );
	mDockToolBars[ Qt::RightToolBarArea ]->toggleViewAction()->setObjectName( "pDockToolBarRightViewAction" );
	mDockToolBars[ Qt::RightToolBarArea ]->toggleViewAction()->setText( tr( "Right toolbar visible" ) );
	mDockToolBars[ Qt::RightToolBarArea ]->toggleExclusiveAction()->setObjectName( "pDockToolBarRightExclusiveAction" );
	mMainWindow->addToolBar( Qt::RightToolBarArea, mDockToolBars[ Qt::RightToolBarArea ] );
	
	foreach ( pDockToolBar* dockToolBar, mDockToolBars ) {
		dockToolBar->hide();
	}
}

void pDockToolBarManager::trackDockWidget( QDockWidget* dockWidget )
{
	Q_ASSERT( dockWidget );
	
	if ( dockWidget->objectName().isEmpty() ) {
		qFatal( qPrintable( QString( "%1: Can't handle dock with no object name" ).arg( Q_FUNC_INFO ) ) );
		return;
	}
	
	connect( dockWidget, SIGNAL( allowedAreasChanged( Qt::DockWidgetAreas ) ), this, SLOT( dockWidget_allowedAreasChanged( Qt::DockWidgetAreas ) ), Qt::UniqueConnection );
	connect( dockWidget, SIGNAL( dockLocationChanged( Qt::DockWidgetArea ) ), this, SLOT( dockWidget_dockLocationChanged( Qt::DockWidgetArea ) ), Qt::UniqueConnection );
	connect( dockWidget, SIGNAL( featuresChanged( QDockWidget::DockWidgetFeatures ) ), this, SLOT( dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures ) ), Qt::UniqueConnection );
	connect( dockWidget, SIGNAL( topLevelChanged( bool ) ), this, SLOT( dockWidget_topLevelChanged( bool ) ), Qt::UniqueConnection );
	connect( dockWidget, SIGNAL( visibilityChanged( bool ) ), this, SLOT( dockWidget_visibilityChanged( bool ) ), Qt::UniqueConnection );
}

void pDockToolBarManager::untrackDockWidget( QDockWidget* dockWidget )
{
	Q_ASSERT( dockWidget );
	
	disconnect( dockWidget, SIGNAL( allowedAreasChanged( Qt::DockWidgetAreas ) ), this, SLOT( dockWidget_allowedAreasChanged( Qt::DockWidgetAreas ) ) );
	disconnect( dockWidget, SIGNAL( dockLocationChanged( Qt::DockWidgetArea ) ), this, SLOT( dockWidget_dockLocationChanged( Qt::DockWidgetArea ) ) );
	disconnect( dockWidget, SIGNAL( featuresChanged( QDockWidget::DockWidgetFeatures ) ), this, SLOT( dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures ) ) );
	disconnect( dockWidget, SIGNAL( topLevelChanged( bool ) ), this, SLOT( dockWidget_topLevelChanged( bool ) ) );
	disconnect( dockWidget, SIGNAL( visibilityChanged( bool ) ), this, SLOT( dockWidget_visibilityChanged( bool ) ) );
}

/*!
	\details Convert a DockWidgetArea to a ToolBarArea and return it.
	\details Return BottomToolBarArea if area is invalid
	\param area The DockWidgetArea to convert
*/
Qt::ToolBarArea pDockToolBarManager::dockWidgetAreaToToolBarArea( Qt::DockWidgetArea area )
{
	switch ( area ) {
		case Qt::LeftDockWidgetArea:
			return Qt::LeftToolBarArea;
			break;
		case Qt::RightDockWidgetArea:
			return Qt::RightToolBarArea;
			break;
		case Qt::TopDockWidgetArea:
			return Qt::TopToolBarArea;
			break;
		case Qt::BottomDockWidgetArea:
			return Qt::BottomToolBarArea;
			break;
		default:
			return Qt::BottomToolBarArea;
			break;
	}
}

/*!
	\details Convert a ToolBarArea to a DockWidgetArea and return it.
	\details Return BottomDockWidgetArea if area is invalid
	\param area The ToolBarArea to convert
*/
Qt::DockWidgetArea pDockToolBarManager::toolBarAreaToDockWidgetArea( Qt::ToolBarArea area )
{
	switch ( area ) {
		case Qt::LeftToolBarArea:
			return Qt::LeftDockWidgetArea;
			break;
		case Qt::RightToolBarArea:
			return Qt::RightDockWidgetArea;
			break;
		case Qt::TopToolBarArea:
			return Qt::TopDockWidgetArea;
			break;
		case Qt::BottomToolBarArea:
			return Qt::BottomDockWidgetArea;
			break;
		default:
			return Qt::BottomDockWidgetArea;
			break;
	}
}

/*!
	\details Convert a ToolBarArea to a QBoxLayout::Direction and return it.
	\details Return QBoxLayout::LeftToRight if area is invalid
	\param area The ToolBarArea to convert
*/
QBoxLayout::Direction pDockToolBarManager::toolBarAreaToBoxLayoutDirection( Qt::ToolBarArea area )
{
	switch ( area ) {
		case Qt::LeftToolBarArea:
			return QBoxLayout::BottomToTop;
			break;
		case Qt::RightToolBarArea:
			return QBoxLayout::TopToBottom;
			break;
		case Qt::TopToolBarArea:
		case Qt::BottomToolBarArea:
			return QBoxLayout::LeftToRight;
			break;
		default:
			return QBoxLayout::LeftToRight;
			break;
	}
}

/*!
	\details Restore the state of the bar given in parameter.
	\param pbar The bar to restore, if null all bars are restored
*/
void pDockToolBarManager::restoreState( pDockToolBar* dockToolBar )
{
	pSettings* settings = mMainWindow->settings();
	QStringList areas;
	
	if ( dockToolBar ) {
		areas << QString::number( mMainWindow->toolBarArea( dockToolBar ) );
	}
	else {
		settings->beginGroup( "MainWindow/Docks" );
		areas = settings->childGroups();
		settings->endGroup();
	}
	
	foreach ( const QString& area, areas ) {
		dockToolBar = this->dockToolBar( Qt::ToolBarArea( area.toInt() ) );
		
		if ( dockToolBar ) {
			const QStringList docksName = settings->value( QString( "MainWindow/Docks/%1/Widgets" ).arg( area ) ).toStringList();
			const bool isExclusive = dockToolBar->exclusive();
			
			dockToolBar->setExclusive( false );
			
			foreach ( const QString& dockName, docksName ) {
				QDockWidget* dockWidget = mMainWindow->findChild<QDockWidget*>( dockName );
				
				if ( dockWidget ) {
					dockToolBar->addDock( dockWidget );
				}
			}
			
			dockToolBar->setExclusive( settings->value( QString( "MainWindow/Docks/%1/Exclusive" ).arg( area ), isExclusive ).toBool() );
		}
	}
}

/*!
	\details Save the state of the bar given in parameter.
	\param bar The bar to save, if null all bars are saved
*/
void pDockToolBarManager::saveState( pDockToolBar* dockToolBar )
{
	pSettings* settings = mMainWindow->settings();
	QList<pDockToolBar*> dockToolBars;
	
	if ( dockToolBar ) {
		dockToolBars << dockToolBar;
	}
	else {
		dockToolBars << mDockToolBars.values();
	}
	
	foreach ( pDockToolBar* dockToolBar, dockToolBars ) {
		QStringList docksName;
		
		foreach ( QDockWidget* dockWidget, dockToolBar->docks( pDockToolBar::InsertedOrder ) ) {
			docksName << dockWidget->objectName();
		}
		
		// write datas
		const int area = mMainWindow->toolBarArea( dockToolBar );
		settings->setValue( QString( "MainWindow/Docks/%1/Exclusive" ).arg( area ), dockToolBar->exclusive() );
		settings->setValue( QString( "MainWindow/Docks/%1/Widgets" ).arg( area ), docksName );
	}
}

void pDockToolBarManager::dockWidget_allowedAreasChanged( Qt::DockWidgetAreas allowedAreas )
{
	Q_UNUSED( allowedAreas );
}

void pDockToolBarManager::dockWidget_dockLocationChanged( Qt::DockWidgetArea area )
{
	QDockWidget* dockWidget = qobject_cast<QDockWidget*>( sender() );
	pDockToolBar* currentDockToolBar = dockToolBar( area );
	
	foreach ( pDockToolBar* dockToolBar, mDockToolBars ) {
		if ( dockToolBar != currentDockToolBar ) {
			dockToolBar->removeDock( dockWidget );
		}
	}
	
	if ( mIsRestoring )
	{
		return;
	}
	
	if ( currentDockToolBar ) {
		currentDockToolBar->addDock( dockWidget );
	}
}

void pDockToolBarManager::dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures features )
{
	Q_UNUSED( features );
}

void pDockToolBarManager::dockWidget_topLevelChanged( bool topLevel )
{
	Q_UNUSED( topLevel );
}

void pDockToolBarManager::dockWidget_visibilityChanged( bool visible )
{
	QDockWidget* dockWidget = qobject_cast<QDockWidget*>( sender() );
	pDockToolBar* dockToolBar = this->dockToolBar( dockWidget );
	
	if ( mIsRestoring )
	{
		return;
	}
	
	if ( dockToolBar ) {
		dockToolBar->setDockVisible( dockWidget, visible );
	}
}

