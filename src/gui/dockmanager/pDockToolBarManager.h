/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pDockToolBarManager.h
** Date      : 2011-02-20T00:41:09
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
#ifndef PDOCKTOOLBARMANAGER_H
#define PDOCKTOOLBARMANAGER_H

/*!
    \file pDockToolBarManager.h
    \brief This class manage a set of pDockToolBar of a pMainWindow according to its mode.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QHash>
#include <QBoxLayout>
#include <QDockWidget>
#include <QToolBar>

class pMainWindow;
class pDockToolBar;
class pSettings;

/*!
    \ingroup FreshGui
    \class pDockToolBarManager
    \brief This class manage a set of pDockToolBar of a pMainWindow according to its mode.
    
    It provide usefull member ( bar() ) to directly create a unique pDockToolBar for the corresponding area.
    It allow to save/restore state of pDockToolBar using a pSettings object.
*/
class FRESH_EXPORT pDockToolBarManager : public QObject
{
    Q_OBJECT
    friend class pDockToolBar;

public:
    /*! This enumeration represents the differents mode available for presenting the toolbars managing the dock widgets. */
    enum Mode {
        Invalid = -1, /*!< An invalid mode. */
        Classic, /*!< The classical mode presenting toolbars at each part of a window (left, top, right, bottom). */
        Modern /*!< The modern mode presenting all the toolbars in a unique toolbar that minimize the used space. */
        
    };
    /*!
        Create a manager working on \a window.
    */
    pDockToolBarManager( pMainWindow* window );
    /*!
        Reimplemented.
    */
    virtual bool eventFilter( QObject* object, QEvent* event );
    /*!
        Return true if settings restoration is in process else false.
    */
    bool isRestoring() const;
    /*!
        Tells if the setttings restoration process is active or not according to \a restoring.
        
        \note This member should never be used directly.
    */
    void setRestoring( bool restoring );
    /*!
        Return the current presentation mode.
    */
    pDockToolBarManager::Mode mode() const;
    /*!
        Set the current presentation mode to \a mode.
    */
    void setMode( pDockToolBarManager::Mode mode );
    /*!
        Return the pMainWindow handled by this manager.
    */
    pMainWindow* mainWindow() const;
    /*!
        Return the toolbar area used by \a toolBar.
    */
    Qt::ToolBarArea toolBarArea( pDockToolBar* toolBar ) const;
    /*!
        Return all the toolbars this manager is using.
    */
    QList<pDockToolBar*> dockToolBars() const;
    /*!
        Return the toolbar placed in the tool bar area \a area.
    */
    pDockToolBar* dockToolBar( Qt::ToolBarArea area ) const;
    /*!
        Return the toolbar placed in the dock widget area \a area.
    */
    pDockToolBar* dockToolBar( Qt::DockWidgetArea area ) const;
    /*!
        Return the pDockToolBar that handles \a dockWidget.
    */
    pDockToolBar* dockToolBar( QDockWidget* dockWidget ) const;
    /*!
        Return pointer to modern dock tool bar widget
     */
    QToolBar* modernToolBar() const;
    /*!
        Conversion helper.
    */
    static Qt::ToolBarArea dockWidgetAreaToToolBarArea( Qt::DockWidgetArea area );
    /*!
        Conversion helper.
    */
    static Qt::DockWidgetArea toolBarAreaToDockWidgetArea( Qt::ToolBarArea area );
    /*!
        Conversion helper.
    */
    static QBoxLayout::Direction toolBarAreaToBoxLayoutDirection( Qt::ToolBarArea area );

protected:
    pMainWindow* mMainWindow;
    QHash<Qt::ToolBarArea, pDockToolBar*> mDockToolBars;
    pDockToolBarManager::Mode mMode;
    QWeakPointer<QToolBar> mModernWidget;
    bool mIsRestoring;
    QTimer* mModernToolBarUpdate;
    /*!
        Initialize the toolbar for each area.
    */
    virtual void initializeToolBars();
    /*!
        Check for unmanaged dock widgets and add them to the correct toolbar.
    */
    virtual void checkForUnManagedDockWidgets();
    /*!
        Change the toolbar \a tb visibility according to \a visible.
    */
    virtual void setToolBarVisible( pDockToolBar* tb, bool visible );
    /*!
        Initialize needed signals / slot / properties for tracking \a dock.
    */
    virtual void trackDockWidget( QDockWidget* dock );
    /*!
        De initialize needed signals / slots / properties for un tracking \a dock.
    */
    virtual void untrackDockWidget( QDockWidget* dock );

public slots:
    /*!
        Restore the state of \a dockToolBar if not null, else all toolbars.
    */
    virtual void restoreState( pDockToolBar* dockToolBar = 0 );
    /*!
        Save the state of \a dockToolBar if not null, else all toolbars.
    */
    virtual void saveState( pDockToolBar* dockToolBar = 0 );

protected slots:
    void updateModernToolBarActions();
    void dockWidget_allowedAreasChanged( Qt::DockWidgetAreas allowedAreas );
    void dockWidget_dockLocationChanged( Qt::DockWidgetArea area );
    void dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures features );
    void dockWidget_topLevelChanged( bool topLevel );
    void dockWidget_visibilityChanged( bool visible );

signals:
    /*!
        This signal is emited when the current prensentation mode is changed to \a mode.
    */
    void modeChanged( pDockToolBarManager::Mode mode );
};

#endif // PDOCKTOOLBARMANAGER_H
