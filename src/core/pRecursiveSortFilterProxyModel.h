/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pRecursiveSortFilterProxyModel.h
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
#ifndef PRECURSIVESORTFILTERPROXYMODEL_H
#define PRECURSIVESORTFILTERPROXYMODEL_H

/*!
    \file pRecursiveSortFilterProxyModel.h
    \brief A QSortFilterProxyModel that does the rows filtering recursively.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QSortFilterProxyModel>

/*!
    \ingroup FreshCore
    \class pRecursiveSortFilterProxyModel
    \brief A QSortFilterProxyModel that does the rows filtering recursively.

    This proxy can filter recursively and handle a source root index.
    The standard QSortFilterProxyModel does not do recursive filtering.
    What is called recursive if that if a node does not match but one of its children match,
    then the parent node will be filtered out making the child not visible.
    This model recursively check for children and does not filter out parent having matching children.
    The purpose of the source root index is to allow to only see a special branch of your model in an optimised way.
    In fact we could simply use QAbstractItemView::setRootIndex to only show the branch we need,
    but the proxy will still filter the entire tree model, which can lead to lag / freeze depending the complexity of the model.
    The source root index allow to avoid the need to set the view rootIndex and the proxy will only filter children of the root index.
    This speed up the filtering mecanism to only thing you are interested in.
*/
class FRESH_EXPORT pRecursiveSortFilterProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  pRecursiveSortFilterProxyModel( QObject* parent = 0 );

  virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

  virtual QModelIndex mapFromSource( const QModelIndex& sourceIndex ) const;
  virtual QModelIndex mapToSource( const QModelIndex& proxyIndex ) const;

  void setRecursiveFilter( bool recursive );
  bool isRecursiveFilter() const;

  void setSourceRootModelIndex( const QModelIndex& index );
  QModelIndex sourceRootModelIndex() const;

protected:
  virtual bool filterAcceptsRowImplementation( int source_row, const QModelIndex & source_parent ) const;

private:
  bool m_recursiveFilter;
  QPersistentModelIndex m_sourceRootIndex;

  virtual bool filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const;
  bool recursiveFilterAcceptsRowImplementation( int source_row, const QModelIndex & source_parent ) const;
};

#endif // PRECURSIVESORTFILTERPROXYMODEL_H
