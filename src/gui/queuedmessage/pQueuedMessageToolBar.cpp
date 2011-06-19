/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pQueuedMessageToolBar.cpp
** Date      : 2011-02-20T00:41:09
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
#include "pQueuedMessageToolBar.h"

#include <QAction>
#include <QPainter>
#include <QLayout>
#include <QEvent>
#include <QDebug>

pQueuedMessageToolBar::pQueuedMessageToolBar( QWidget* parent )
	: QToolBar( parent )
{
	mQueuedWidget = new pQueuedMessageWidget( this );
	
	setObjectName( metaObject()->className() );
	setMovable( false );
	setFloatable( false );
	setAllowedAreas( Qt::TopToolBarArea );
	toggleViewAction()->setEnabled( false );
	toggleViewAction()->setVisible( false );
	
	addWidget( mQueuedWidget );
	layout()->setMargin( 3 );
	
	// connections
	connect( mQueuedWidget, SIGNAL( shown( const pQueuedMessage& ) ), this, SLOT( messageShown( const pQueuedMessage& ) ) );
	connect( mQueuedWidget, SIGNAL( finished() ), this, SLOT( messageFinished() ) );
}

pQueuedMessageWidget* pQueuedMessageToolBar::queuedMessageWidget() const
{
	return mQueuedWidget;
}

void pQueuedMessageToolBar::changeEvent( QEvent* event )
{
	if ( event->type() == QEvent::FontChange ) {
		mQueuedWidget->setFont( font() );
	}
	
	QToolBar::changeEvent( event );
}

void pQueuedMessageToolBar::paintEvent( QPaintEvent* event )
{
	if ( mQueuedWidget->pendingMessageCount() == 0 ) {
		QToolBar::paintEvent( event );
		return;
	}
	
	const QBrush brush = mQueuedWidget->currentMessageBackground();
	QPainter painter( this );
	painter.setPen( brush.color().darker( 150 ) );
	painter.setBrush( brush );
	painter.drawRect( contentsRect().adjusted( 0, 0, -1, -1 ) );
}

pQueuedMessage pQueuedMessageToolBar::appendMessage( const QString& message, int milliSeconds )
{
	return mQueuedWidget->append( message, milliSeconds );
}

void pQueuedMessageToolBar::appendMessage( const pQueuedMessage& message )
{
	mQueuedWidget->append( message );
}

void pQueuedMessageToolBar::removeMessage( const pQueuedMessage& message )
{
	mQueuedWidget->remove( message );
}

void pQueuedMessageToolBar::messageShown( const pQueuedMessage& message )
{
	Q_UNUSED( message );
	
	if ( !isVisible() ) {
		setVisible( true );
	}
}

void pQueuedMessageToolBar::messageFinished()
{
	if ( isVisible() ) {
		setVisible( false );
	}
}
