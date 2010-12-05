#include "pDockToolBarManager.h"
#include "pDockToolBarManagerModernWidget.h"
#include "gui/pMainWindow.h"
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
	mMode = pDockToolBarManager::Invalid;
	mMainWindow = window;
	mIsRestoring = false;
	initializeToolBars();
	mMainWindow->installEventFilter( this );
}

bool pDockToolBarManager::eventFilter( QObject* object, QEvent* event )
{
	switch ( event->type() ) {
		case QEvent::ChildPolished: {
			QChildEvent* ce = static_cast<QChildEvent*>( event );
			QDockWidget* dock = qobject_cast<QDockWidget*>( ce->child() );
			
			if ( dock ) {
				trackDockWidget( dock );
			}
			
			break;
		}
		case QEvent::ChildRemoved: {
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

pDockToolBarManager::Mode pDockToolBarManager::mode() const
{
	return mMode;
}

void pDockToolBarManager::setMode( pDockToolBarManager::Mode mode )
{
	if ( mode == mMode ) {
		return;
	}
	
	mMode = mode;
	
	switch ( mMode ) {
		case pDockToolBarManager::Classic:
			mMainWindow->addToolBar( Qt::TopToolBarArea, mDockToolBars[ Qt::TopToolBarArea ] );
			mMainWindow->addToolBar( Qt::BottomToolBarArea, mDockToolBars[ Qt::BottomToolBarArea ] );
			mMainWindow->addToolBar( Qt::LeftToolBarArea, mDockToolBars[ Qt::LeftToolBarArea ] );
			mMainWindow->addToolBar( Qt::RightToolBarArea, mDockToolBars[ Qt::RightToolBarArea ] );
			mDockToolBars[ Qt::TopToolBarArea ]->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
			mDockToolBars[ Qt::BottomToolBarArea ]->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
			mDockToolBars[ Qt::LeftToolBarArea ]->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
			mDockToolBars[ Qt::RightToolBarArea ]->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
			
			if ( !mMainWindow->toolBarBreak( mDockToolBars[ Qt::TopToolBarArea ] ) ) {
				mMainWindow->insertToolBarBreak( mDockToolBars[ Qt::TopToolBarArea ] );
			}
			
			break;
		case pDockToolBarManager::Modern: {
			mModernWidget.data()->setToolBars( mDockToolBars.values() );
			break;
		}
		default:
			break;
	}
	
	mModernWidget.data()->setVisible( mMode == pDockToolBarManager::Modern );
	bool visible = false;
	
	foreach ( pDockToolBar* toolBar, mDockToolBars ) {
		toolBar->internal_updateButtonsState();
		toolBar->setVisible( false );
		
		if ( toolBar->count() > 0 ) {
			visible = true;
			toolBar->setVisible( true );
		}
	}
	
	mModernWidget.data()->setVisible( visible && mMode == pDockToolBarManager::Modern );
	
	emit modeChanged( mMode );
}

/*!
	\details Return the associated pMainWindow
*/
pMainWindow* pDockToolBarManager::mainWindow() const
{
	return mMainWindow;
}

Qt::ToolBarArea pDockToolBarManager::toolBarArea( pDockToolBar* toolBar ) const
{
	return mDockToolBars.key( toolBar );
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
		if ( dockToolBar->hasDockWidget( dockWidget ) ) {
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
	
	// Qt::BottomToolBarArea
	mDockToolBars[ Qt::BottomToolBarArea ] = new pDockToolBar( this, Qt::Horizontal );
	mDockToolBars[ Qt::BottomToolBarArea ]->setObjectName( "pDockToolBarBottom" );
	mDockToolBars[ Qt::BottomToolBarArea ]->setWindowTitle( tr( "Bottom toolbar" ) );
	mDockToolBars[ Qt::BottomToolBarArea ]->toggleViewAction()->setObjectName( "pDockToolBarBottomViewAction" );
	mDockToolBars[ Qt::BottomToolBarArea ]->toggleViewAction()->setText( tr( "Bottom toolbar visible" ) );
	mDockToolBars[ Qt::BottomToolBarArea ]->toggleExclusiveAction()->setObjectName( "pDockToolBarBottomExclusiveAction" );
	
	// Qt::LeftToolBarArea
	mDockToolBars[ Qt::LeftToolBarArea ] = new pDockToolBar( this, Qt::Vertical );
	mDockToolBars[ Qt::LeftToolBarArea ]->setObjectName( "pDockToolBarLeft" );
	mDockToolBars[ Qt::LeftToolBarArea ]->setWindowTitle( tr( "Left toolbar" ) );
	mDockToolBars[ Qt::LeftToolBarArea ]->toggleViewAction()->setObjectName( "pDockToolBarLeftViewAction" );
	mDockToolBars[ Qt::LeftToolBarArea ]->toggleViewAction()->setText( tr( "Left toolbar visible" ) );
	mDockToolBars[ Qt::LeftToolBarArea ]->toggleExclusiveAction()->setObjectName( "pDockToolBarLeftExclusiveAction" );
	
	// Qt::RightToolBarArea
	mDockToolBars[ Qt::RightToolBarArea ] = new pDockToolBar( this, Qt::Vertical );
	mDockToolBars[ Qt::RightToolBarArea ]->setObjectName( "pDockToolBarRight" );
	mDockToolBars[ Qt::RightToolBarArea ]->setWindowTitle( tr( "Right toolbar" ) );
	mDockToolBars[ Qt::RightToolBarArea ]->toggleViewAction()->setObjectName( "pDockToolBarRightViewAction" );
	mDockToolBars[ Qt::RightToolBarArea ]->toggleViewAction()->setText( tr( "Right toolbar visible" ) );
	mDockToolBars[ Qt::RightToolBarArea ]->toggleExclusiveAction()->setObjectName( "pDockToolBarRightExclusiveAction" );
	
	// modern widget manager
	mModernWidget = new pDockToolBarManagerModernWidget( mMainWindow );
	mMainWindow->addToolBar( Qt::LeftToolBarArea, mModernWidget.data() );
}

void pDockToolBarManager::checkForUnManagedDockWidgets()
{
	if ( !mMainWindow ) {
		return;
	}
	
	foreach ( QDockWidget* dockWidget, mMainWindow->findChildren<QDockWidget*>() ) {
		bool hasDockWidget = false;
		
		foreach ( pDockToolBar* toolBar, mDockToolBars ) {
			if ( toolBar->hasDockWidget( dockWidget ) ) {
				hasDockWidget = true;
				break;
			}
		}
		
		if ( hasDockWidget ) {
			continue;
		}
		
		dockToolBar( mMainWindow->dockWidgetArea( dockWidget ) )->addDockWidget( dockWidget );
	}
}

void pDockToolBarManager::setToolBarVisible( pDockToolBar* tb, bool visible )
{
	if ( !mDockToolBars.values().contains( tb ) ) {
		return;
	}
	
	if ( visible ) {
		if ( mMode == pDockToolBarManager::Modern && visible ) {
			if ( !mModernWidget.data()->isVisible() ) {
				mModernWidget.data()->setVisible( true );
			}
		}
		
		tb->setVisible( true );
	}
	else {
		tb->setVisible( false );
		
		if ( mMode == pDockToolBarManager::Modern ) {
			bool visible = false;
			
			foreach ( pDockToolBar* toolBar, mDockToolBars ) {
				if ( toolBar->isVisible() ) {
					visible = true;
					break;
				}
			}
			
			mModernWidget.data()->setVisible( visible );
		}
	}
}

void pDockToolBarManager::trackDockWidget( QDockWidget* dockWidget )
{
	Q_ASSERT( dockWidget );
	
	if ( dockWidget->objectName().isEmpty() ) {
		qFatal( "%s", qPrintable( QString( "%1: Can't handle dock with no object name" ).arg( Q_FUNC_INFO ) ) );
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
		case Qt::RightDockWidgetArea:
			return Qt::RightToolBarArea;
		case Qt::TopDockWidgetArea:
			return Qt::TopToolBarArea;
		case Qt::BottomDockWidgetArea:
			return Qt::BottomToolBarArea;
		default:
			return Qt::BottomToolBarArea;
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
		case Qt::RightToolBarArea:
			return Qt::RightDockWidgetArea;
		case Qt::TopToolBarArea:
			return Qt::TopDockWidgetArea;
		case Qt::BottomToolBarArea:
			return Qt::BottomDockWidgetArea;
		default:
			return Qt::BottomDockWidgetArea;
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
		case Qt::RightToolBarArea:
			return QBoxLayout::TopToBottom;
		case Qt::TopToolBarArea:
		case Qt::BottomToolBarArea:
			return QBoxLayout::LeftToRight;
		default:
			return QBoxLayout::LeftToRight;
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
		areas << QString::number( mDockToolBars.key( dockToolBar ) );
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
					dockToolBar->addDockWidget( dockWidget );
				}
			}
			
			dockToolBar->setExclusive( settings->value( QString( "MainWindow/Docks/%1/Exclusive" ).arg( area ), isExclusive ).toBool() );
		}
	}
	
	checkForUnManagedDockWidgets();
	setMode( pDockToolBarManager::Mode( settings->value( "MainWindow/Docks/Mode", pDockToolBarManager::Modern ).toInt() ) );
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
		
		foreach ( QDockWidget* dockWidget, dockToolBar->dockWidgets( pDockToolBar::InsertedOrder ) ) {
			docksName << dockWidget->objectName();
		}
		
		// write datas
		const int area = mDockToolBars.key( dockToolBar );
		settings->setValue( QString( "MainWindow/Docks/%1/Exclusive" ).arg( area ), dockToolBar->exclusive() );
		settings->setValue( QString( "MainWindow/Docks/%1/Widgets" ).arg( area ), docksName );
	}
	
	settings->setValue( "MainWindow/Docks/Mode", mMode );
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
			dockToolBar->removeDockWidget( dockWidget );
		}
	}
	
	if ( mIsRestoring ) {
		return;
	}
	
	if ( currentDockToolBar ) {
		currentDockToolBar->addDockWidget( dockWidget );
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
	
	if ( mIsRestoring ) {
		return;
	}
	
	if ( dockToolBar ) {
		dockToolBar->setDockVisible( dockWidget, visible );
	}
}
