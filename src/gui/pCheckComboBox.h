/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pCheckComboBox.h
** Date      : 2011-02-20T00:44:25
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
#ifndef PCHECKCOMBOBOX_H
#define PCHECKCOMBOBOX_H

/*!
    \file pCheckComboBox.h
    \brief A checkable combo box.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pComboBox.h"

#include <QStyledItemDelegate>

class QEvent;
class QListView;

class pGenericTableModel;

class FRESH_EXPORT pCheckComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    pCheckComboBoxDelegate( QObject* parent, QComboBox* cmb );

    static bool isSeparator( const QModelIndex& index );
    static void setSeparator( QAbstractItemModel* model, const QModelIndex& index, bool set );

protected:
    virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;

private:
    QComboBox *mCombo;
};

/*!
    \ingroup FreshGui
    \class pCheckComboBox
    \brief A checkable combo box.

    This combo box add the ability to present a combo box where items are checkable.
    The inactive state show a comma separated list of each checked items.
    In addition, the combobox has usefull members for dealing with items separators.
*/
class FRESH_EXPORT pCheckComboBox : public pComboBox
{
    Q_OBJECT
    
public:
    /*!
        Construct a pLocaleModel having parent \a parent.
    */
    pCheckComboBox( QWidget* parent = 0 );
    /*!
        Reimplemented.
    */
    virtual void showPopup();
    /*!
        Insert a separator at \a index.
    */
    void insertSeparator( int index );
    /*!
        Add a separator at end.
    */
    void addSeparator();
    /*!
        Return true if the item at \a index is a separator.
    */
    bool isSeparator( int index );
    /*!
        Set the item at \a index a separator according to \a set.
    */
    void setSeparator( int index, bool set = true );
    /*!
        Return the list of checked indexes.
    */
    QList<int> checkedRows() const;
    /*!
        Return the list of checked indexes text.
    */
    QStringList checkedStringList() const;
    /*!
        Return the list of checked model indexes.
    */
    QModelIndexList checkedIndexes() const;
    /*!
        Clear all check states.
    */
    void clearCheckStates();

public slots:
    /*!
        Retranslate the items.
    */
    virtual void retranslateUi();
    
protected:
    pGenericTableModel* mModel;
    QListView* mView;
    pCheckComboBoxDelegate* mDelegate;
    
    /*!
        Reimplemented.
    */
    virtual bool event( QEvent* event );
    /*!
        Reimplemented.
    */
    virtual void changeEvent( QEvent* event );
    /*!
        Reimplemented.
    */
    virtual void paintEvent( QPaintEvent* event );
    
    QModelIndex modelIndex( int index ) const;
    QString text() const;
};

#endif // PCHECKCOMBOBOX_H
