#ifndef PMAINWINDOW_H
#define PMAINWINDOW_H

/*!
	\file pMainWindow.h
	\brief An extended QMainWindow.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"

#include <QMainWindow>

class pSettings;
class pActionsNodeMenuBar;
class pQueuedMessageToolBar;
class pDockToolBarManager;
class pDockToolBar;

/*!
	\ingroup Gui
	\class pMainWindow
	\brief An extended QMainWindow.

	This extended mainwindow contains some usefull features :
	- a pMenuBar as menu bar.
	- a pDockToolBarManager for tabbed management of docked widgets.
	- a pSettings object for storing/reading your settings.
	
	There is also a confortable dockToolBar() member to deal with the pDockToolBar.
*/
class FRESH_EXPORT pMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/*!
		Create an instance of the main window having \a parent as parent and window flags \a windowFlags.
	*/
	pMainWindow( QWidget* parent = 0, Qt::WindowFlags windowFlags = 0 );
	/*!
		Reimplemented.
	*/
	virtual ~pMainWindow();
	/*!
		Reimplemented.
	*/
	virtual QMenu* createPopupMenu();
	/*!
		Return the settings object used by this main window.
	*/
	virtual pSettings* settings() const;
	/*!
		Return the menu bar object.
	*/
	virtual pActionsNodeMenuBar* menuBar() const;
	/*!
		Return the queued message toolbar object.
	*/
	virtual pQueuedMessageToolBar* queuedMessageToolBar() const;
	/*!
		Return the dock widget toolbar manager object.
	*/
	virtual pDockToolBarManager* dockToolBarManager() const;
	/*!
		Return the dock toolbar manager for \a area.
	*/
	pDockToolBar* dockToolBar( Qt::ToolBarArea area ) const;

protected:
	bool mShown;
	mutable pSettings* mSettings;
	mutable pActionsNodeMenuBar* mMenuBar;
	mutable pQueuedMessageToolBar* mQueuedMessageToolBar;
	mutable pDockToolBarManager* mDockToolBarManager;
	/*!
		Reimplemented.
	*/
	virtual void showEvent( QShowEvent* event );
	/*!
		Reimplemented.
	*/
	virtual void closeEvent( QCloseEvent* event );

public slots:
	/*!
		Save the state of the main window.
		
		\sa QMainWindow::saveState().
	*/
	virtual void saveState();
	/*!
		Restore the state of the main window.
		
		\sa QMainWindow::restoreState().
	*/
	virtual void restoreState();
};

#endif // PMAINWINDOW_H
