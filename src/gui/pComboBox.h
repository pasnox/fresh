/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pComboBox.h
** Date      : 2012-11-20T00:44:25
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
#ifndef PCOMBOBOX_H
#define PCOMBOBOX_H

/*!
    \file pComboBox.h
    \brief A combo box with specialized signals.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QComboBox>

/*!
    \ingroup FreshGui
    \class pComboBox
    \brief A combo box with specialized signals.

    This combo box add the ability to informs by signals that the popup is about to show / hide.
*/
class FRESH_EXPORT pComboBox : public QComboBox
{
    Q_OBJECT
    
public:
    /*!
        Construct a pLocaleModel having parent \a parent.
    */
    pComboBox( QWidget* parent = 0 );
    /*!
        Reimplemented.
    */
    virtual void hidePopup();
    /*!
        Reimplemented.
    */
    virtual void showPopup();

signals:
    /*!
        This signal is emited just before the popup is hidden.
    */
    void popupAboutToHide();
    /*!
        This signal is emited just before the popup is shown.
    */
    void popupAboutToShow();
};

#endif // PCOMBOBOX_H
