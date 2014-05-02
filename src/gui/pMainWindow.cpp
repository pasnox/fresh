/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pMainWindow.cpp
** Date      : 2011-02-20T00:41:35
** License   : LGPL v3
** Home Page : https://github.com/pasnox/fresh
** Comment   : Fresh Library is a Qt 4 extension library providing set of new core & gui classes.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Leser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "pMainWindow.h"
#include "pSettings.h"
#include "actionmanager/pActionsMenuBar.h"
#include "queuedmessage/pQueuedMessageToolBar.h"
#include "dockmanager/pDockToolBarManager.h"
#include "dockmanager/pDockToolBar.h"

#include <QMenu>

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

pDockToolBarManager* pMainWindow::dockToolBarManager() const
{
    if ( !mDockToolBarManager ) {
        mDockToolBarManager = new pDockToolBarManager( const_cast<pMainWindow*>( this ) );
    }
    
    return mDockToolBarManager;
}

pDockToolBar* pMainWindow::dockToolBar( Qt::ToolBarArea area ) const
{
    return dockToolBarManager()->dockToolBar( area );
}

pSettings* pMainWindow::settings() const
{
    if ( !mSettings ) {
        mSettings = new pSettings( const_cast<pMainWindow*>( this ) );
    }
    
    return mSettings;
}

pActionsMenuBar* pMainWindow::menuBar() const
{
    if ( !mMenuBar ) {
#if defined( Q_OS_MAC )
        mMenuBar = new pActionsMenuBar;
#else
        mMenuBar = new pActionsMenuBar( const_cast<pMainWindow*>( this ) );
#endif
    }
    
    return mMenuBar;
}

void pMainWindow::addDockManagerWidget( Qt::ToolBarArea area, QDockWidget* dockWidget, Qt::Orientation orientation )
{
    dockToolBar( area )->addDockWidget( dockWidget );
    
    if ( orientation == Qt::Horizontal ) {
        dockWidget->setFeatures( dockWidget->features() ^ QDockWidget::DockWidgetVerticalTitleBar );
    }
    else {
        dockWidget->setFeatures( dockWidget->features() | QDockWidget::DockWidgetVerticalTitleBar );
    }
}

void pMainWindow::setDockManagerAreaExclusive( Qt::ToolBarArea area, bool exclusive )
{
    dockToolBar( area )->setExclusive( exclusive );
}

void pMainWindow::setDockManagerMode( pDockToolBarManager::Mode mode )
{
    dockToolBarManager()->setMode( mode );
}

void pMainWindow::appendMessage( const QString& message, int milliSeconds )
{
    queuedMessageToolBar()->appendMessage( message, milliSeconds );
}

void pMainWindow::appendMessage( const pQueuedMessage& message )
{
    queuedMessageToolBar()->appendMessage( message );
}

void pMainWindow::saveState()
{
    if ( settings() ) {
        dockToolBarManager()->saveState();
        settings()->saveState( this );
    }
}

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
