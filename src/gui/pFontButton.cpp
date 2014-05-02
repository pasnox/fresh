/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pFontButton.cpp
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
#include "pFontButton.h"

#include <QEvent>
#include <QFontDialog>
#include <QStyleOptionButton>
#include <QPainter>
#include <QStyle>
#include <QApplication>

pFontButton::pFontButton( QWidget* parent )
    : QPushButton( parent )
{
    init( font() );
}

pFontButton::pFontButton( const QFont& font, QWidget* parent )
    : QPushButton( parent )
{
    init( font );
}

bool pFontButton::event( QEvent* event )
{
    if ( event->type() == QEvent::FontChange ) {
        updateFont();
    }
    
    return QPushButton::event( event );
}

void pFontButton::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );
    QPainter painter( this );
    QStyleOptionButton option;
    initStyleOption( &option );
    painter.setFont( QFont( font().family(), QApplication::font().pointSize() ) );
    style()->drawControl( QStyle::CE_PushButton, &option, &painter, 0 );
}

void pFontButton::updateFont()
{
    const QFont f = font();
    
    setText( f.family() );
    emit fontChanged( f );
}

void pFontButton::init( const QFont& font )
{
    const QSize sh = sizeHint();
    setMaximumSize( sh.width() *4, sh.height() );
    setFont( font );
    updateFont();
    connect( this, SIGNAL( clicked() ), this, SLOT( _q_clicked() ) );
}

void pFontButton::_q_clicked()
{
    bool ok = false;
    const QFont font = QFontDialog::getFont( &ok, this->font(), this, tr( "Choose a font" ), QFontDialog::FontDialogOptions( 0 ) );
    
    if ( ok ) {
        setFont( font );
    }
}
