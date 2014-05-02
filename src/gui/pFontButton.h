/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pFontButton.h
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
#ifndef PFONTBUTTON_H
#define PFONTBUTTON_H

/*!
    \file pFontButton.h
    \brief A QPushButton that allow to choose a QFont.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QPushButton>

/*!
    \ingroup FreshGui
    \class pFontButton
    \brief A QPushButton that allow to choose a QFont.
*/
class FRESH_EXPORT pFontButton : public QPushButton
{
    Q_OBJECT
    
public:
    /*!
        Create an instance of button having parent \a parent.
    */
    pFontButton( QWidget* parent = 0 );
    /*!
        Create an instance of button having parent \a parent
        and initialized with \a font.
    */
    pFontButton( const QFont& font, QWidget* parent = 0 );

protected:
    virtual bool event( QEvent* event );
    virtual void paintEvent( QPaintEvent* event );
    
    void updateFont();
    void init( const QFont& font );

protected slots:
    void _q_clicked();

signals:
    /*!
        This signal is emited when the font has changed.
    */
    void fontChanged( const QFont& font );
};

#endif // PFONTBUTTON_H
