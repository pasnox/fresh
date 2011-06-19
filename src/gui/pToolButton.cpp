/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pToolButton.cpp
** Date      : 2011-02-20T00:41:35
** License   : LGPL v3
** Home Page : https://github.com/pasnox/fresh
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
#include "pToolButton.h"

#include <QPaintEvent>
#include <QMouseEvent>
#include <QStyleOptionToolButton>
#include <QToolBar>
#include <QMenu>
#include <QPainter>
#include <QApplication>

pToolButton::pToolButton( QWidget* parent, QBoxLayout::Direction direction )
	: QToolButton( parent )
{
	mMenuDown = false;
	
	setDirection( direction );
}

QSize pToolButton::internalSize( Qt::Orientation orientation ) const
{
	QSize size = QToolButton::size();
	Qt::Orientation buttonOrientation = Qt::Horizontal;
	
	switch ( mDirection ) {
		case QBoxLayout::TopToBottom:
		case QBoxLayout::BottomToTop:
			buttonOrientation = Qt::Vertical;
			break;
		default:
			break;
	}
	
	if ( buttonOrientation != orientation ) {
		size.transpose();
	}
	
	return size;
}

void pToolButton::paint( QStyleOptionToolButton* option )
{
	Q_ASSERT( option );
	
	QPainter painter( this );
	QTransform transform;
	
	// fix some properties due to rotation not handled by Qt
	switch ( cursorArea() ) {
		case pToolButton::caButtonClicked:
			option->activeSubControls |= QStyle::SC_ToolButton;
			
			if ( popupMode() == QToolButton::MenuButtonPopup ) {
				option->state |= QStyle::State_MouseOver;
				option->activeSubControls |= QStyle::SC_ToolButtonMenu;
			}
			
			break;
		default:
			break;
	}
	
	switch ( mDirection ) {
		case QBoxLayout::TopToBottom:
			option->rect.setSize( internalSize( Qt::Horizontal ) );
			transform.rotate( 90 );
			transform.translate( 0, -option->rect.height() +1 );
			break;
		case QBoxLayout::BottomToTop:
			option->rect.setSize( internalSize( Qt::Horizontal ) );
			transform.rotate( -90 );
			transform.translate( -option->rect.width() +1, 0 );
			break;
		default:
			break;
	}
	
	painter.setTransform( transform );
	
	style()->drawComplexControl( QStyle::CC_ToolButton, option, &painter, this );
}

void pToolButton::paintEvent( QPaintEvent* event )
{
	Q_UNUSED( event );
	
	QStyleOptionToolButton option;
	initStyleOption( &option );
	
	paint( &option );
}

void pToolButton::mousePressEvent( QMouseEvent* event )
{
	switch ( cursorArea( event->pos() ) ) {
		case pToolButton::caArrowClicked:
			mMenuDown = true;
			showMenu();
			mMenuDown = false;
			break;
		case pToolButton::caButtonClicked:
			setDown( !isDown() );
			break;
		case pToolButton::caNone:
			break;
		default:
			QAbstractButton::mousePressEvent( event );
			break;
	}
	
	// update button
	update();
}

void pToolButton::mouseMoveEvent( QMouseEvent* event )
{
	QAbstractButton::mouseMoveEvent( event );

	if ( event->buttons() != Qt::NoButton ) {
		setDown( hitButton( event->pos() ) );
	}
}

void pToolButton::mouseReleaseEvent( QMouseEvent* event )
{
	mMenuDown = false;

	switch ( cursorArea( event->pos() ) ) {
		case pToolButton::caButton:
			click();
			break;
		case pToolButton::caArrow:
		case pToolButton::caNone:
			break;
		default:
			QAbstractButton::mouseReleaseEvent( event );
			break;
	}

	// update button
	update();
}

pToolButton::CursorArea pToolButton::cursorArea( const QPoint& _pos ) const
{
	// cursor pos
	const QPoint pos = _pos.isNull() ? mapFromGlobal( QCursor::pos() ) : _pos;

	// if not contain is button return none
	if ( !hitButton( pos ) ) {
		return pToolButton::caNone;
	}

	// is arrow type
	bool arrowType = popupMode() == QToolButton::MenuButtonPopup;

	// is mouse pressed ?!
	bool mousePressed = QApplication::mouseButtons() & Qt::LeftButton;

	// check if we are a arrow button
	if ( arrowType ) {
		// get bounding rectangle
		QRect rect = this->rect();
	
		// get style options
		QStyleOptionToolButton opt;
		initStyleOption( &opt );
	
		// force to do horizontal calcul
		opt.rect.setSize( internalSize( Qt::Horizontal ) );

		// get arraow bounding rectangle
		QSize size = style()->subControlRect( QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButtonMenu, this ).size();

		switch ( mDirection ) {
			case QBoxLayout::BottomToTop:
				size.transpose();
				break;
			case QBoxLayout::TopToBottom:
				size.transpose();
				rect.setY( rect.height() -size.height() );
				break;
			default:
				rect.setX( rect.width() -size.width() );
				break;
		}
	
		// get valid bounding rectangle size
		rect.setSize( size );

		// in arrow bounding rect
		if ( rect.isValid() && rect.contains( pos ) ) {
			return mousePressed ? pToolButton::caArrowClicked : pToolButton::caArrow;
		}
	}

	// in button
	return mousePressed ? pToolButton::caButtonClicked : pToolButton::caButton;
}

QMenu* pToolButton::hasMenu() const
{
	QMenu* menu = this->menu();
	
	if ( !menu && defaultAction() ) {
		menu = defaultAction()->menu();
	}
	
	return menu;
}

bool pToolButton::menuButtonDown() const
{
#ifndef QT_NO_MENU
	return hasMenu() && mMenuDown;
#else
	return false;
#endif
}

QSize pToolButton::minimumSizeHint() const
{
	return QSize();
}

QSize pToolButton::sizeHint() const
{
	//get default size
	QSize size = QToolButton::sizeHint();

	// calcul new size hint
	switch ( mDirection ) {
		case QBoxLayout::LeftToRight:
		case QBoxLayout::RightToLeft:
			break;
		case QBoxLayout::TopToBottom:
		case QBoxLayout::BottomToTop:
			size.transpose();
			break;
	}

	// return new size hint
	return size;
}

QBoxLayout::Direction pToolButton::direction() const
{
	return mDirection;
}

void pToolButton::setDirection( QBoxLayout::Direction direction )
{
	if ( mDirection == direction ) {
		return;
	}
	
	mDirection = direction;
	updateGeometry();
}

QVariant pToolButton::userData() const
{
	return mUserData;
}

void pToolButton::setUserData( const QVariant& data )
{
	mUserData = data;
}
