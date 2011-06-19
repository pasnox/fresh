/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pDockWidgetTitleBar.h
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
#ifndef PDOCKWIDGETTITLEBAR_H
#define PDOCKWIDGETTITLEBAR_H

/*!
	\file pDockWidgetTitleBar.h
	\brief A custom title bar for pDockWidget that herits QToolBar.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QToolBar>
#include <QDockWidget>

class QToolButton;
class pToolButton;
class QStyleOptionToolButton;

/*!
	\ingroup FreshGui
	\class pDockWidgetTitleBar
	\brief A custom title bar for pDockWidget that herits QToolBar.

	Actions can be inserted in the title bar like in any toolbar !
*/
class FRESH_EXPORT pDockWidgetTitleBar : public QToolBar
{
	Q_OBJECT

public:
	/*!
		Create an instance of the title bar handing parent has \a parent.
	*/
	pDockWidgetTitleBar( QDockWidget* parent = 0 );
	/*!
		Reimplemented.
	*/
	virtual QSize minimumSizeHint() const;
	/*!
		Reimplemented.
	*/
	virtual QSize sizeHint() const;
	/*!
		Insert \a action at \a index position.
		If \a index is -1 then it's append at end.
	*/
	QWidget* addAction( QAction* action, int index = -1 );
	/*!
		Insert a separator at \a index.
		If \a index is -1 then it's append at end.
	*/
	void addSeparator( int index = -1 );
	/*!
		Set the rendering to be native or custom according to \a native.
	*/
	void setNativeRendering( bool native );
	/*!
		Return true if native rendering is done else false.
	*/
	bool nativeRendering() const;
	
	/*!
		Set the default rendering to be native or custom according to \a native.
	*/
	static void setDefaultNativeRendering( bool native );
	/*!
		Return true if default native rendering is done else false.
	*/
	static bool defaultNativeRendering();

protected:
	QDockWidget* mDock;
	QAction* aOrientation;
	QAction* aFloat;
	QAction* aClose;
	pToolButton* tbOrientation;
	pToolButton* tbFloat;
	pToolButton* tbClose;
	bool mUseNativePaint;
	static bool mUseNativePaintDefault;
	/*!
		Reimplemented
	*/
	virtual bool event( QEvent* event );
	/*!
		Reimplemented.
	*/
	virtual bool eventFilter( QObject* object, QEvent* event );
	/*!
		Reimplemented
	*/
	virtual void paintEvent( QPaintEvent* event );
	
	QIcon icon() const;
	QSize windowIconSize() const;
	void updateStyleChange();

protected slots:
	void aOrientation_triggered();
	void aFloat_triggered();
	void dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures features );
};

#endif // PDOCKWIDGETTITLEBAR_H
