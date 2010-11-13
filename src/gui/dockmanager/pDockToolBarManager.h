#ifndef PDOCKTOOLBARMANAGER_H
#define PDOCKTOOLBARMANAGER_H

/*!
	\file pDockToolBarManager.h
	\date 2008-01-14T00:27:41
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief This class manage a set of pDockToolBar ( left, top, right and bottom ) of a QMainWindow
*/

#include "core/FreshExport.h"

#include <QHash>
#include <QBoxLayout>
#include <QDockWidget>

class pMainWindow;
class pDockToolBar;
class pSettings;
class pDockToolBarManagerModernWidget;

/*!
	\brief This class manage a set of pDockToolBar ( left, top, right and bottom ) of a QMainWindow
	\details It provide usefull member ( bar() ) to directly create a unique pDockToolBar for the corresponding area.
	\details It allow to save/restore state of pDockToolBar using a pSettings class.
*/
class FRESH_EXPORT pDockToolBarManager : public QObject
{
	Q_OBJECT
	friend class pDockToolBar;

public:
	enum Mode {
		Invalid = -1,
		Classic,
		Modern
		
	};
	
	pDockToolBarManager( pMainWindow* window );
	
	virtual bool eventFilter( QObject* object, QEvent* event );
	
	bool isRestoring() const;
	void setRestoring( bool restoring );
	
	pDockToolBarManager::Mode mode() const;
	void setMode( pDockToolBarManager::Mode mode );
	
	pMainWindow* mainWindow() const;
	Qt::ToolBarArea toolBarArea( pDockToolBar* toolBar ) const;
	QList<pDockToolBar*> dockToolBars() const;
	pDockToolBar* dockToolBar( Qt::ToolBarArea area ) const;
	pDockToolBar* dockToolBar( Qt::DockWidgetArea area ) const;
	pDockToolBar* dockToolBar( QDockWidget* dockWidget ) const;

	static Qt::ToolBarArea dockWidgetAreaToToolBarArea( Qt::DockWidgetArea area );
	static Qt::DockWidgetArea toolBarAreaToDockWidgetArea( Qt::ToolBarArea area );
	static QBoxLayout::Direction toolBarAreaToBoxLayoutDirection( Qt::ToolBarArea area );

protected:
	pMainWindow* mMainWindow;
	QHash<Qt::ToolBarArea, pDockToolBar*> mDockToolBars;
	pDockToolBarManager::Mode mMode;
	QWeakPointer<pDockToolBarManagerModernWidget> mModernWidget;
	bool mIsRestoring;
	
	virtual void initializeToolBars();
	virtual void checkForUnManagedDockWidgets();
	virtual void setToolBarVisible( pDockToolBar* tb, bool visible );
	virtual void trackDockWidget( QDockWidget* dock );
	virtual void untrackDockWidget( QDockWidget* dock );

public slots:
	virtual void restoreState( pDockToolBar* dockToolBar = 0 );
	virtual void saveState( pDockToolBar* dockToolBar = 0 );

protected slots:
	void dockWidget_allowedAreasChanged( Qt::DockWidgetAreas allowedAreas );
	void dockWidget_dockLocationChanged( Qt::DockWidgetArea area );
	void dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures features );
	void dockWidget_topLevelChanged( bool topLevel );
	void dockWidget_visibilityChanged( bool visible );

signals:
	void modeChanged( pDockToolBarManager::Mode mode );
};

#endif // PDOCKTOOLBARMANAGER_H
