/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pColorButton.cpp
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
#include "pColorButton.h"
#include "pGuiUtils.h"

#include <QColorDialog>

pColorButton::pColorButton( QWidget* parent )
    : QToolButton( parent )
{
    init( QColor( Qt::black ) );
}

pColorButton::pColorButton( const QColor& color, QWidget* parent )
    : QToolButton( parent )
{
    init( color );
}

void pColorButton::init( const QColor& color )
{
    mAlphaEnabled = color.alpha() != 255;
    setIconSize( QSize( 16, 16 ) );
    setColor( color );
    connect( this, SIGNAL( clicked() ), this, SLOT( _q_clicked() ) );
}

QColor pColorButton::color() const
{
    QColor color = mColor;

    if ( !mAlphaEnabled ) {
        color.setAlpha( 255 );
    }

    return color;
}

void pColorButton::setColor( const QColor& color )
{
    setDefaultColor( color );
    emit colorChanged( mColor );
}

void pColorButton::setDefaultColor( const QColor& color )
{
    mColor = color;
    mColor = this->color(); // remove alpha if needed

    const QStringList texts = QStringList()
        << QString( "RGBA #%1%2%3%4" ).arg( mColor.red(), 2, 16, QChar( '0' ) ).arg( mColor.green(), 2, 16, QChar( '0' ) ).arg( mColor.blue(), 2, 16, QChar( '0' ) ).arg( mColor.alpha(), 2, 16, QChar( '0' ) )
        << QString( "RGBA %1, %2, %3, %4" ).arg( mColor.red() ).arg( mColor.green() ).arg( mColor.blue() ).arg( mColor.alpha() )
        ;

    setText( texts.first() );
    setToolTip( texts.join( "\n" ) );

    setIcon( QIcon( pGuiUtils::filledPixmap( mColor, iconSize() ) ) );
}

bool pColorButton::alphaEnabled() const
{
    return mAlphaEnabled;
}

void pColorButton::setAlphaEnabled( bool enabled )
{
    mAlphaEnabled = enabled;

    const QColor c = color();

    if ( mColor != c ) {
        emit colorChanged( c );
    }
}

void pColorButton::_q_clicked()
{
    const QColor color = QColorDialog::getColor( mColor, window(), tr( "Choose a color" ), mAlphaEnabled ? QColorDialog::ShowAlphaChannel : QColorDialog::ColorDialogOptions( 0 ) );

    if ( color.isValid() ) {
        setColor( color );
    }
}
