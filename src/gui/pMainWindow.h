/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pMainWindow.h
** Date      : 2011-02-20T00:41:35
** License   : LGPL v3
** Home Page : http://bettercodes.org/projects/fresh
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
#ifndef PMAINWINDOW_H
#define PMAINWINDOW_H

/*!
	\file pMainWindow.h
	\brief An extended QMainWindow.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pQueuedMessageWidget.h"
#include "pDockToolBarManager.h"

#include <QMainWindow>

class pSettings;
class pActionsMenuBar;
class pQueuedMessageToolBar;
class pDockToolBar;

/*!
	\ingroup FreshGui
	\class pMainWindow
	\brief An extended QMainWindow.

	This extended mainwindow contains some usefull features :
	- a pMenuBar as menu bar.
	- a pDockToolBarManager for tabbed management of docked widgets.
	- a pQueuedMessageToolBar for present the user passive errors / messages.
	- a pSettings object for storing/reading your settings.
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
	virtual pActionsMenuBar* menuBar() const;
	/*!
		Add \a dockWidget in \a area having \a orientation.
	*/
	void addDockManagerWidget( Qt::ToolBarArea area, QDockWidget* dockWidget, Qt::Orientation orientation = Qt::Horizontal );
	/*!
		Seth the dock toolbar in \a area to be exclusive according to \a exclusive.
	*/
	void setDockManagerAreaExclusive( Qt::ToolBarArea area, bool exclusive );
	/*!
		Set the dock manager presentation mode to \a mode.
	*/
	void setDockManagerMode( pDockToolBarManager::Mode mode );
	/*!
		Append a queued \a message in the message toolbar with an auto close defined to \a milliSeconds.
		
		\sa pQueuedMessageToolBar::appendMessage( const QString&, int ).
	*/
	void appendMessage( const QString& message, int milliSeconds = -1 );
	/*!
		Append a queued \a message in the message toolbar.
		
		\sa pQueuedMessageToolBar::appendMessage( const pQueuedMessage& ).
	*/
	void appendMessage( const pQueuedMessage& message );

protected:
	bool mShown;
	mutable pSettings* mSettings;
	mutable pActionsMenuBar* mMenuBar;
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
