#ifndef PMAINWINDOW_H
#define PMAINWINDOW_H

/*!
	\file pMainWindow.h
	\date 2008-01-14T00:27:46
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief An extended QMainWindow
*/

#include "core/FreshExport.h"

#include <QMainWindow>

class pActionsNodeMenuBar;
class pDockToolBarManager;
class pDockToolBar;
class pSettings;

/*!
	\brief An extended QMainWindow.
	\details This extended mainwindow contains some usefull features :
	\details a pMenuBar as menu bar, a pDockToolBarManager for tabbed management of docked widgets
	\details and a pSettings object for storing/reading your settings.
	\details There is also a confortable dockToolBar() member.
*/
class FRESH_EXPORT pMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	pMainWindow( QWidget* parent = 0, Qt::WindowFlags windowFlags = 0 );
	virtual ~pMainWindow();
	
	virtual QMenu* createPopupMenu();
	
	virtual pSettings* settings() const;
	virtual pActionsNodeMenuBar* menuBar() const;
	virtual pDockToolBarManager* dockToolBarManager() const;
	pDockToolBar* dockToolBar( Qt::ToolBarArea area ) const;

protected:
	mutable pSettings* mSettings;
	mutable pActionsNodeMenuBar* mMenuBar;
	mutable pDockToolBarManager* mDockToolBarManager;

	virtual void showEvent( QShowEvent* event );
	virtual void closeEvent( QCloseEvent* event );

public slots:
	virtual void saveState();
	virtual void restoreState();
};

#endif // PMAINWINDOW_H
