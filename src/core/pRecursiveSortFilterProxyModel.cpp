/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pRecursiveSortFilterProxyModel.cpp
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
#include "pRecursiveSortFilterProxyModel.h"

pRecursiveSortFilterProxyModel::pRecursiveSortFilterProxyModel( QObject* parent )
  : QSortFilterProxyModel( parent )
  , m_recursiveFilter( false )
{
}

QVariant pRecursiveSortFilterProxyModel::data( const QModelIndex& index, int role ) const
{
  if ( m_sourceRootIndex.isValid() ) {
    if ( index == QModelIndex() ) {
      return QVariant();
    }
  }

  return QSortFilterProxyModel::data( index, role );
}

QModelIndex pRecursiveSortFilterProxyModel::mapFromSource( const QModelIndex& sourceIndex ) const
{
  if ( m_sourceRootIndex.isValid() ) {
    if ( sourceIndex == m_sourceRootIndex /*|| !sourceIndex.isValid()*/ ) {
      return QModelIndex();
    }
  }

  return QSortFilterProxyModel::mapFromSource( sourceIndex );
}

QModelIndex pRecursiveSortFilterProxyModel::mapToSource( const QModelIndex& proxyIndex ) const
{
  if ( m_sourceRootIndex.isValid() ) {
    if ( proxyIndex == QModelIndex() /*|| !proxyIndex.isValid()*/ ) {
      return m_sourceRootIndex;
    }
  }

  return QSortFilterProxyModel::mapToSource( proxyIndex );
}

void pRecursiveSortFilterProxyModel::setRecursiveFilter( bool recursive )
{
  beginResetModel();
  m_recursiveFilter = recursive;
  endResetModel();
}

bool pRecursiveSortFilterProxyModel::isRecursiveFilter() const
{
  return m_recursiveFilter;
}

void pRecursiveSortFilterProxyModel::setSourceRootModelIndex( const QModelIndex& index )
{
  beginResetModel();
  m_sourceRootIndex = index;
  endResetModel();
}

QModelIndex pRecursiveSortFilterProxyModel::sourceRootModelIndex() const
{
  return m_sourceRootIndex;
}

bool pRecursiveSortFilterProxyModel::filterAcceptsRowImplementation( int source_row, const QModelIndex& source_parent ) const
{
  return QSortFilterProxyModel::filterAcceptsRow( source_row, source_parent );
}

bool pRecursiveSortFilterProxyModel::filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const
{
  const QModelIndex index = sourceModel()->index( source_row, 0, source_parent );

  if ( m_sourceRootIndex.isValid() ) {
    QModelIndex parent = index.parent();

    while ( parent != m_sourceRootIndex && parent.isValid() ) {
      parent = parent.parent();
    }

    if ( parent != m_sourceRootIndex ) {
      return false;
    }
  }

  if ( m_recursiveFilter ) {
    return recursiveFilterAcceptsRowImplementation( source_row, source_parent );
  }
  else {
    return filterAcceptsRowImplementation( source_row, source_parent );
  }
}

bool pRecursiveSortFilterProxyModel::recursiveFilterAcceptsRowImplementation( int source_row, const QModelIndex& source_parent ) const
{
  if ( filterAcceptsRowImplementation( source_row, source_parent ) ) {
      return true;
  }

  const QModelIndex index = sourceModel()->index( source_row, 0, source_parent );
  const int count = sourceModel()->rowCount( index );

  for ( int i = 0; i < count; i++ ) {
    if ( filterAcceptsRowImplementation( i, index ) ) {
      return true;
    }
    else {
      return recursiveFilterAcceptsRowImplementation( i, index );
    }
  }

  return false;
}
