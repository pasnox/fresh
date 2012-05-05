/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pRecursiveSortFilterProxyModel.cpp
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
#include "pRecursiveSortFilterProxyModel.h"

pRecursiveSortFilterProxyModel::pRecursiveSortFilterProxyModel( QObject* parent )
    : QSortFilterProxyModel( parent )
{
}

bool pRecursiveSortFilterProxyModel::filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const
{
    const QModelIndex index = sourceModel()->index( source_row, 0, source_parent );
    const int rowCount = sourceModel()->rowCount( index );
    const bool accepted = QSortFilterProxyModel::filterAcceptsRow( source_row, source_parent );
    
    if ( rowCount > 0 && !accepted ) {
        for ( int row = 0; row < rowCount; row++ ) {
            if ( filterAcceptsRow( row, index ) ) {
                return true;
            }
        }
    }
    
    return accepted;
}
