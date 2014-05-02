/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pDockWidget.cpp
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
#include "pDockWidget.h"
#include "pDockWidgetTitleBar.h"

#include <QPainter>
#include <QStyle>
#include <QTimer>
#include <QAction>

pDockWidget::pDockWidget( const QString& title, QWidget* parent, Qt::WindowFlags flags )
    : QDockWidget( title, parent, flags )
{
    init();
}

pDockWidget::pDockWidget( QWidget* parent, Qt::WindowFlags flags )
    : QDockWidget( parent, flags )
{
    init();
}

void pDockWidget::init()
{
    mShowAction = NULL;
    mTitleBar = new pDockWidgetTitleBar( this );
    setTitleBarWidget( mTitleBar );
    connect( toggleViewAction(), SIGNAL( triggered( bool ) ), this, SLOT( toggleViewAction_triggered( bool ) ) );
}

void pDockWidget::paintEvent( QPaintEvent* event )
{
    QDockWidget::paintEvent(  event );
    
    if ( isFloating() && style()->objectName().compare( "Oxygen", Qt::CaseInsensitive ) != 0 ) {
        QRect rect = this->rect().adjusted( 0, 0, -1, -1 );
        
        QPainter painter( this );
        painter.setPen( QColor( 145, 142, 142 ) );
        painter.setBrush( Qt::NoBrush );
        painter.drawRect( rect );
    }
}

pDockWidgetTitleBar* pDockWidget::titleBar() const
{
    return mTitleBar;
}

QAction* pDockWidget::showAction()
{
    if ( !mShowAction )
    {
        mShowAction = new QAction(windowIcon(), windowTitle(), this);
        connect(mShowAction, SIGNAL(activated()), this, SLOT(show()));
        connect(mShowAction, SIGNAL(activated()), this, SLOT(handleFocusProxy()));
    }
    return mShowAction;
}

void pDockWidget::toggleViewAction_triggered( bool toggled )
{
    if ( toggled && focusProxy() ) {
        if ( isFloating() ) {
            QTimer::singleShot( 0, this, SLOT( handleWindowActivation() ) );
        }
        else {
            QTimer::singleShot( 0, this, SLOT( handleFocusProxy() ) );
        }
    }
}

void pDockWidget::handleWindowActivation()
{
    activateWindow();
    QTimer::singleShot( 0, this, SLOT( handleFocusProxy() ) );
}

void pDockWidget::handleFocusProxy()
{
    if ( focusProxy() )
        focusProxy()->setFocus();
}
