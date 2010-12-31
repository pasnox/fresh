#ifndef PDOCKTOOLBARMANAGER_H
#define PDOCKTOOLBARMANAGER_H

/*!
	\file pDockToolBarManager.h
	\brief This class manage a set of pDockToolBar of a pMainWindow according to its mode.
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
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
	\ingroup Gui
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
		Set the current presentation mode.
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
