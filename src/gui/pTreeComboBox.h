/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pTreeComboBox.h
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
#ifndef PTREECOMBOBOX_H
#define PTREECOMBOBOX_H

/*!
    \file pTreeComboBox.h
    \brief A special combobox handling a QTreeView as popup widget.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QWidget>
#include <QPersistentModelIndex>
#include <QPointer>

class QFrame;
class QVBoxLayout;
class QTreeView;
class QAbstractItemModel;
class QStyleOptionComboBox;

/*!
    \ingroup FreshGui
    \class pTreeComboBox
    \brief A special combobox handling a QTreeView as popup widget.
*/
class FRESH_EXPORT pTreeComboBox : public QWidget
{
    Q_OBJECT

public:
    /*!
        Create an instance of the combobox having \a parent as parent.
    */
    pTreeComboBox( QWidget* parent = 0 );
    /*!
        Reimplemented.
    */
    virtual bool eventFilter( QObject* object, QEvent* event );
    /*!
        Reimplemented.
    */
    virtual QSize sizeHint() const;
    /*!
        Return the size of the icon.
    */
    QSize iconSize() const;
    /*!
        Set the size of the icon to \a size.
        
        \note The view has to be defined to take effect.
    */
    void setIconSize( const QSize& size );
    /*!
        This member is responsable of cleaning the state of the widget when hiding the popup widget.
    */
    virtual void hidePopup();
    /*!
        This member is responsable of initializing the state of the widget to popup the widget.
    */
    virtual void showPopup();
    /*!
        Return the tree view used by this combobox.
    */
    QTreeView* view() const;
    /*!
        Set the view used by the combobox.
        
        \note The old view is deleted.
    */
    virtual void setView( QTreeView* view );
    /*!
        return the model used by the view.
    */
    QAbstractItemModel* model() const;
    /*!
        Set the view model to \a model.
        
        \note The view has to be defined to take effect.
    */
    virtual void setModel( QAbstractItemModel* model );
    /*!
        Return the model column used to draw text / icon when the tree is not popuped.
    */
    int modelColumn() const;
    /*!
        Set the model column to used for drawinf the text / icon when the tree is not popuped.
    */
    void setColumnModel( int column );
    /*!
        Return the view root index.
    */
    QModelIndex rootIndex() const;
    /*!
        Set te view root index to \a index.
        
        \note The view has to be defined to take effect.
    */
    void setRootIndex( const QModelIndex& index );
    /*!
        Return the view current index.
    */
    QModelIndex currentIndex() const;
    /*!
        Set the view current index to \a index.
        
        \note The view has to be defined to take effect.
    */
    void setCurrentIndex( const QModelIndex& index );
    /*!
        Expand all the node of the view.
    */
    void expandAll();

protected slots:
    void internal_activated( const QModelIndex& index );
    void internal_clicked( const QModelIndex& index );
    void internal_currentChanged( const QModelIndex& current, const QModelIndex& previous );

protected:
    int mModelColumn;
    int mMaxVisibleItems;
    QFrame* mFrame;
    QVBoxLayout* mFrameLayout;
    QPointer<QTreeView> mView;
    QPersistentModelIndex mCurrentIndex;
    bool mForceIndex;

    /*!
        This member follow the Qt style of initializing an option style for the combobox.
    */
    virtual void initStyleOption( QStyleOptionComboBox* option ) const;
    /*!
        Reimplemented.
    */
    virtual bool event( QEvent* event );
    /*!
        Reimplemented.
    */
    virtual void paintEvent( QPaintEvent* event );
    /*!
        Reimplemented.
    */
    virtual void hideEvent( QHideEvent* event );
    /*!
        Reimplemented.
    */
    virtual void mousePressEvent( QMouseEvent* event );

    void calculPopupGeometry();
    QModelIndex modelColumnIndex( const QModelIndex& index ) const;

signals:
    /*!
        This signal is emit when \a index is activated.
        
        \sa QTreeView::activated( const QModelIndex& ).
    */
    void activated( const QModelIndex& index );
    /*!
        This signal is emit when \a index is clicked.
        
        \sa QTreeView::clicked( const QModelIndex& ).
    */
    void clicked( const QModelIndex& index );
    /*!
        This signal is emit when the current index changed to \a index.
    */
    void currentIndexChanged( const QModelIndex& index );
    /*!
        This signal is emit when \a index is highlighted.
        
        \sa QTreeView::highlighted( const QModelIndex& ).
    */
    void highlighted( const QModelIndex& index );
};

#endif // PTREECOMBOBOX_H
