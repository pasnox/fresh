/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pToolButton.h
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
#ifndef PTOOLBUTTON_H
#define PTOOLBUTTON_H

/*!
	\file pToolButton.h
	\brief An extended QToolButton that can be rotated.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QToolButton>
#include <QBoxLayout>
#include <QVariant>

class QStyleOptionToolButton;

/*!
	\ingroup Gui
	\class pToolButton
	\brief An extended QToolButton that can be rotated.

	This button can be rotated.
*/
class FRESH_EXPORT pToolButton : public QToolButton
{
	Q_OBJECT
	friend class pDockWidgetTitleBar;

public:
	/*!
		Create an instance of button having \a parent as parent and layouted in \a direction.
	*/
	pToolButton( QWidget* parent, QBoxLayout::Direction direction = QBoxLayout::LeftToRight );
	/*!
		Reimplemented.
	*/
	virtual QSize minimumSizeHint() const;
	/*!
		Reimplemented.
	*/
	virtual QSize sizeHint() const;
	/*!
		Return the direction this button is layouted.
	*/
	QBoxLayout::Direction direction() const;
	/*!
		Return the user data associated with this button.
	*/
	QVariant userData() const;

public slots:
	/*!
		Set the direction of the button to \a direction.
	*/
	void setDirection( QBoxLayout::Direction direction );
	/*!
		Set the user data to \a data.
	*/
	void setUserData( const QVariant& data );

protected:
	QBoxLayout::Direction mDirection;
	bool mMenuDown;
	QVariant mUserData;
	
	enum CursorArea {
		caNone = 0,
		caArrow,
		caButton,
		caArrowClicked,
		caButtonClicked
	};
	/*!
		Reimplemented.
	*/
	virtual void paintEvent( QPaintEvent* event );
	/*!
		Reimplemented.
	*/
	virtual void mousePressEvent( QMouseEvent* event );
	/*!
		Reimplemented.
	*/
	virtual void mouseMoveEvent( QMouseEvent* event );
	/*!
		Reimplemented.
	*/
	virtual void mouseReleaseEvent( QMouseEvent* event );

	void paint( QStyleOptionToolButton* option );
	QSize internalSize( Qt::Orientation orientation ) const;
	pToolButton::CursorArea cursorArea( const QPoint& pos = QPoint() ) const;

	QMenu* hasMenu() const;
	bool menuButtonDown() const;
};

#endif // PTOOLBUTTON_H
