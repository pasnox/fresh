/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pDockToolBarManagerModernWidget.h
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
#ifndef PDOCKTOOLBARMANAGERMODERNWIDGET_H
#define PDOCKTOOLBARMANAGERMODERNWIDGET_H

/*!
	\file pDockToolBarManagerModernWidget.h
	\brief This is the modern widget used in the pDockToolBarManager when in Modern mode.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QToolBar>

class QScrollArea;
class QBoxLayout;

class pDockToolBar;

/*!
	\ingroup FreshGui
	\class pDockToolBarManagerModernWidget
	\brief This is the modern widget used in the pDockToolBarManager when in Modern mode.
*/
class FRESH_EXPORT pDockToolBarManagerModernWidget : public QToolBar
{
	Q_OBJECT
	friend class pDockToolBarManager;
	
public:
	/*!
		Create a pDockToolBarManagerModernWidget having parent \a parent.
	*/
	pDockToolBarManagerModernWidget( QWidget* parent = 0 );
	/*!
		Destroyes the widget.
	*/
	virtual ~pDockToolBarManagerModernWidget();
	/*!
		Reimplemented.
	*/
	virtual QSize sizeHint() const;
	/*!
		Set the internal toolbars handled by the modern widget..
	*/
	void setToolBars( const QList<pDockToolBar*>& toolBars );

protected:
	QScrollArea* mArea;
	QWidget* mContent;
	QBoxLayout* mLayout;

protected slots:
	void _q_orientationChanged( Qt::Orientation orientation );
};

#endif // PDOCKTOOLBARMANAGERMODERNWIDGET_H
