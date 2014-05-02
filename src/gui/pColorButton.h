/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pColorButton.h
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
#ifndef PCOLORBUTTON_H
#define PCOLORBUTTON_H

/*!
    \file pColorButton.h
    \brief A QToolButton that allow to choose a QColor.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QToolButton>

/*!
    \ingroup FreshGui
    \class pColorButton
    \brief A QToolButton that allow to choose a QColor.
*/
class FRESH_EXPORT pColorButton : public QToolButton
{
    Q_OBJECT
    /*!
    \property pColorButton::color
    \brief the color shown on the button.

    If the button has no color, the color() function will return an anvalid QColor.

    The default color is Qt::black.
    */
    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
    /*!
    \property pColorButton::alphaEnabled
    \brief the color alpha is enabled.

    The alpha edition will be enabled depending this property.

    The default value if false is alpha is 255 else true.
    */
    Q_PROPERTY( bool alphaEnabled READ alphaEnabled WRITE setAlphaEnabled )
    
public:
    /*!
        Create an instance of button having parent \a parent.
    */
    pColorButton( QWidget* parent = 0 );
    /*!
        Create an instance of button having parent \a parent
        and initialized with \a color.
    */
    pColorButton( const QColor& color, QWidget* parent = 0 );
    /*!
        Return the color.
    */
    QColor color() const;
    /*!
        Return the color alpha enabled state.
    */
    bool alphaEnabled() const;

protected:
    QColor mColor;
    bool mAlphaEnabled;
    
    void init( const QColor& color );

public slots:
    /*!
        Set the color to \a color.
    */
    void setColor( const QColor& color );
    /*!
        Set the color alpha enabled to \a enabled.
    */
    void setAlphaEnabled( bool enabled );

protected slots:
    void _q_clicked();

signals:
    /*!
        This signal is emited when the color has changed.
    */
    void colorChanged( const QColor& color );
};

#endif // PCOLORBUTTON_H
