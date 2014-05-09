/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pLocaleSortFilterProxyModel.cpp
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
#include "pLocaleSortFilterProxyModel.h"
#include "pLocaleModel.h"

pLocaleSortFilterProxyModel::pLocaleSortFilterProxyModel( QObject* parent )
  : QSortFilterProxyModel( parent )
{
}

void pLocaleSortFilterProxyModel::setSourceModel( QAbstractItemModel* sourceModel )
{
  pLocaleModel* localeModel = qobject_cast<pLocaleModel*>( sourceModel );

  Q_ASSERT( localeModel );

  if ( !localeModel ) {
    qWarning( "You can only set a pLocaleModel as sourceModel." );
    return;
  }

  QSortFilterProxyModel::setSourceModel( localeModel );
}

QStringList pLocaleSortFilterProxyModel::acceptedLocales() const
{
  return mAcceptedLocales.toList();
}

void pLocaleSortFilterProxyModel::setAcceptedLocales( const QStringList& locales )
{
  mAcceptedLocales = locales.toSet();
  invalidateFilter();
}

bool pLocaleSortFilterProxyModel::filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const
{
  const pLocaleModel* localeModel = qobject_cast<pLocaleModel*>( sourceModel() );
  const QModelIndex index = localeModel->index( source_row, 0, source_parent );
  const QString locale = localeModel->indexToLocale( index );
  return mAcceptedLocales.contains( locale );
}
