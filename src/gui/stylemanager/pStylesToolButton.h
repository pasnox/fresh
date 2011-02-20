/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pStylesToolButton.h
** Date      : 2011-02-20T00:41:10
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
#ifndef PSTYLESTOOLBUTTON_H
#define PSTYLESTOOLBUTTON_H

/*!
	\file pStylesToolButton.h
	\brief A special QToolButton that has a QMenu of available styles.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QToolButton>

class QMenu;
class pStylesActionGroup;

/*!
	\ingroup FreshGui
	\class pStylesToolButton
	\brief A special QToolButton that has a QMenu of available styles.
*/
class FRESH_EXPORT pStylesToolButton : public QToolButton
{
	Q_OBJECT

public:
	/*!
		Create an instance of the tool button having parent \a parent.
	*/
	pStylesToolButton( QWidget* parent = 0 );
	/*!
		Create an instance of the tool button having parent \a parent and action text format \a textFormat.
		\sa pStylesActionGroup::pStylesActionGroup( const QString&, QObject* )
	*/
	pStylesToolButton( const QString& textFormat, QWidget* parent = 0 );
	/*!
		\sa pStylesActionGroup::isCheckable()
	*/
	bool isCheckableActions() const;
	/*!
		\sa pStylesActionGroup::currentStyle()
	*/
	QString currentStyle() const;
	/*!
		\sa pStylesActionGroup::systemAction()
	*/
	QAction* systemAction() const;
	/*!
		\sa pStylesActionGroup::applicationAction()
	*/
	QAction* applicationAction() const;
	/*!
		\sa pStylesActionGroup::systemStyle()
	*/
	QString systemStyle() const;
	/*!
		\sa pStylesActionGroup::applicationStyle()
	*/
	QString applicationStyle() const;

public slots:
	/*!
		\sa pStylesActionGroup::setCheckable( bool )
	*/
	void setCheckableActions( bool checkable );
	/*!
		\sa pStylesActionGroup::setCurrentStyle( const QString& )
	*/
	void setCurrentStyle( const QString& style );

protected:
	QMenu* mMenu;
	pStylesActionGroup* mActions;
	
	void init( const QString& textFormat );

signals:
	/*!
		\sa pStylesActionGroup::styleSelected( const QString& );
	*/
	void styleSelected( const QString& style );
};

#endif // PSTYLESTOOLBUTTON_H
