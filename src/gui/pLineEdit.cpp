/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pLineEdit.cpp
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
#include "pLineEdit.h"
#include "pIconManager.h"

#include <QToolButton>
#include <QStyleOptionFrameV3>
#include <QPainter>
#include <QTimer>
#include <QEvent>
#include <QDebug>

const int pLineEdit::mTimeOut = 1000;

pLineEdit::pLineEdit( QWidget* parent )
	: QLineEdit( parent )
{
	init();
}

pLineEdit::pLineEdit( const QString& contents, QWidget* parent )
	: QLineEdit( contents, parent )
{
	init();
}

QMenu* pLineEdit::menu() const
{
	return tbSearch->menu();
}

void pLineEdit::setMenu( QMenu* menu )
{
	tbSearch->setMenu( menu );
}

bool pLineEdit::isSearchButtonVisible() const
{
	return tbSearch->isVisible();
}

QString pLineEdit::promptText() const
{
	return mPromptText;
}

void pLineEdit::setSearchButtonVisible( bool visible )
{
	tbSearch->setVisible( visible );
	
	int left;
	int top;
	int right;
	int bottom;
	
	getTextMargins( &left, &top, &right, &bottom );
	
	if ( visible ) {
		left = mMargin +mSpacing;
	}
	else {
		left = 0;
	}
	
	setTextMargins( left, top, right, bottom );
}

void pLineEdit::setPromptText( const QString& prompt )
{
	mPromptText = prompt;
	update();
}

void pLineEdit::clickSearchButton()
{
	tbSearch->click();
}

void pLineEdit::clickClearButton()
{
	tbClear->click();
}

void pLineEdit::paintEvent( QPaintEvent* event )
{
	QLineEdit::paintEvent( event );
	
	if ( !mPromptText.isEmpty() && text().isEmpty() && isEnabled() ) {
		QStyleOptionFrameV3 option;
		initStyleOption( &option );
		
		int left;
		int top;
		int right;
		int bottom;
		
		getTextMargins( &left, &top, &right, &bottom );
		
		const Qt::Alignment va = style()->visualAlignment( layoutDirection(), alignment() );
		const QRect rect = style()->subElementRect( QStyle::SE_LineEditContents, &option, this ).adjusted( 2, 0, 0, 0 ).adjusted( left, top, -right, -bottom );
		const QFontMetrics fm( font() );
		const QString text = fm.elidedText( mPromptText, Qt::ElideRight, rect.width() );
		QPainter painter( this );
		
		painter.setPen( palette().color( QPalette::Disabled, QPalette::Text ) );
		painter.drawText( rect, va, text );
	}
}

void pLineEdit::resizeEvent( QResizeEvent* event )
{
	QLineEdit::resizeEvent( event );
	
	tbSearch->resize( QSize( mMargin, height() -2 ) );
	tbSearch->move( 3, 1 );
	tbClear->resize( QSize( mMargin, height() -2 ) );
	tbClear->move( width() -mMargin -3, 0 );
}

void pLineEdit::init()
{
	mMargin = sizeHint().height() -2;
	mSpacing = 0;
	
	tbSearch = new QToolButton( this );
	tbSearch->setIcon( pIconManager::icon( "search.png", ":/fresh" ) );
	tbSearch->setToolTip( tr( "Search Options" ) );
	tbSearch->setStyleSheet( "QToolButton { border: none; padding: 0px; } QToolButton::menu-indicator { right: -2px; bottom: -2px; }" );
	tbSearch->setCursor( Qt::ArrowCursor );
	tbSearch->setFocusPolicy( Qt::NoFocus );
	tbSearch->setPopupMode( QToolButton::InstantPopup );
	
	tbClear = new QToolButton( this );
	tbClear->setIcon( pIconManager::icon( "edit-clear-rtl.png", ":/fresh" ) );
	tbClear->setToolTip( tr( "Clear" ) );
	tbClear->setStyleSheet( "QToolButton { border: none; padding: 0px; }" );
	tbClear->setCursor( Qt::ArrowCursor );
	tbClear->setFocusPolicy( Qt::NoFocus );
	
	setSearchButtonVisible( true );
	setClearButtonVisible( false );
	setPromptText( "Search..." );
	
	mTimer = new QTimer( this );
	mTimer->setInterval( mTimeOut );
	mTimer->setSingleShot( true );
	
	connect( this, SIGNAL( textChanged( const QString& ) ), this, SLOT( _q_textChanged( const QString& ) ) );
	connect( tbSearch, SIGNAL( clicked() ), this, SIGNAL( searchButtonClicked() ) );
	connect( tbSearch, SIGNAL( triggered( QAction* ) ), this, SIGNAL( searchButtonActionTriggered( QAction* ) ) );
	connect( tbClear, SIGNAL( clicked() ), this, SLOT( clear() ) );
	connect( tbClear, SIGNAL( clicked() ), this, SIGNAL( clearButtonClicked() ) );
	connect( mTimer, SIGNAL( timeout() ), this, SLOT( timer_timeout() ) );
}

void pLineEdit::setClearButtonVisible( bool visible )
{
	tbClear->setVisible( visible );
	
	int left;
	int top;
	int right;
	int bottom;
	
	getTextMargins( &left, &top, &right, &bottom );
	
	if ( visible ) {
		right = mMargin +mSpacing;
	}
	else {
		right = 0;
	}
	
	setTextMargins( left, top, right, bottom );
}

void pLineEdit::_q_textChanged( const QString& text )
{
	setClearButtonVisible( !text.isEmpty() );
	mTimer->start();
}

void pLineEdit::timer_timeout()
{
	emit searchTextChanged( text() );
}
