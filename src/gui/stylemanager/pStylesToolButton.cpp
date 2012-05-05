/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pStylesToolButton.cpp
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
#include "pStylesToolButton.h"
#include "pStylesActionGroup.h"

#include <QMenu>

pStylesToolButton::pStylesToolButton( QWidget* parent )
    : QToolButton( parent )
{
    init( QLatin1String( "%1" ) );
}

pStylesToolButton::pStylesToolButton( const QString& textFormat, QWidget* parent )
    : QToolButton( parent )
{
    init( textFormat );
}

bool pStylesToolButton::isCheckableActions() const
{
    return mActions->isCheckable();
}

QString pStylesToolButton::currentStyle() const
{
    return mActions->currentStyle();
}

QAction* pStylesToolButton::systemAction() const
{
    return mActions->systemAction();
}

QAction* pStylesToolButton::applicationAction() const
{
    return mActions->applicationAction();
}

QString pStylesToolButton::systemStyle() const
{
    return mActions->systemStyle();
}

QString pStylesToolButton::applicationStyle() const
{
    return mActions->applicationStyle();
}

void pStylesToolButton::setCheckableActions( bool checkable )
{
    mActions->setCheckable( checkable );
}

void pStylesToolButton::setCurrentStyle( const QString& style )
{
    mActions->setCurrentStyle( style );
}

void pStylesToolButton::init( const QString& textFormat )
{
    mActions = new pStylesActionGroup( textFormat, this );
    mMenu = new QMenu( this );
    
    //mActions->updateActions();
    mActions->installInMenu( mMenu );
    
    setMenu( mMenu );
    setText( mActions->systemAction()->text() );
    setPopupMode( QToolButton::MenuButtonPopup );
    
    connect( this, SIGNAL( clicked() ), mActions->systemAction(), SLOT( trigger() ) );
    connect( mActions, SIGNAL( styleSelected(const QString& ) ), this, SIGNAL( styleSelected(const QString& ) ) );
}
