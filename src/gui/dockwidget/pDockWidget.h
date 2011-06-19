/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pDockWidget.h
** Date      : 2011-02-20T00:41:09
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
#ifndef PDOCKWIDGET_H
#define PDOCKWIDGET_H

/*!
	\file pDockWidget.h
	\brief An extended QDockWidget class that handle a QToolBar as title bar widget.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QDockWidget>
#include <QAction>

class pDockWidgetTitleBar;

/*!
	\ingroup FreshGui
	\class pDockWidget
	\brief An extended QDockWidget class.

	This dock widget has a QToolBar as title bar widget and handle its proxy widget on activation.
	The dock widget has a small frame when it's floating (except for Oxygen style).
*/
class FRESH_EXPORT pDockWidget : public QDockWidget
{
	Q_OBJECT
	friend class pDockWidgetTitleBar;

public:
	/*!
		Create a pDockWidget having \a title, \a parent and \a flags.
	*/
	pDockWidget( const QString& title, QWidget* parent = 0, Qt::WindowFlags flags = 0 );
	/*!
		Create a pDockWidget having \a parent and \a flags.
	*/
	pDockWidget( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
	/*!
		Return the title bar widget.
	*/
	pDockWidgetTitleBar* titleBar() const;
	
	/*!
		Show widget
		Unlike toggleViewAction, this action only shows widget, but never hides
	 */
	QAction* showAction();

protected:
	pDockWidgetTitleBar* mTitleBar;
	QAction* mShowAction;

	void init();
	/*!
		Reimplemented.
	*/
	virtual void paintEvent( QPaintEvent* event );

protected slots:
	void toggleViewAction_triggered( bool toggled );
	void handleWindowActivation();
	void handleFocusProxy();
};

#endif // PDOCKWIDGET_H
