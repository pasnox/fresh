/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pLocaleSortFilterProxyModel.h
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
#ifndef PLOCALESORTFILTERPROXYMODEL_H
#define PLOCALESORTFILTERPROXYMODEL_H

/*!
    \file pLocaleSortFilterProxyModel.h
    \brief A proxy model for filtering pLocaleModel.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QSortFilterProxyModel>
#include <QStringList>
#include <QSet>

/*!
    \ingroup FreshCore
    \class pLocaleSortFilterProxyModel
    \brief A proxy model for filtering pLocaleModel.

    This proxy model provide a filtering proxy for a pLocaleModel.
*/
class FRESH_EXPORT pLocaleSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
  
public:
    /*!
        Construct a pLocaleModel having parent \a parent.
    */
    pLocaleSortFilterProxyModel( QObject* parent = 0 );
    /*!
        Reimplemented.
    */
    virtual void setSourceModel( QAbstractItemModel* sourceModel );
    /*!
        Return the accepted locales.
    */
    QStringList acceptedLocales() const;
    
public slots:
    /*!
        Set the accepted locales to \a locales.
        If \a locales is empty (the default), no locales will be shown.
    */
    void setAcceptedLocales( const QStringList& locales );

protected:
    QSet<QString> mAcceptedLocales;

    /*!
        Reimplemented.
    */
    virtual bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const;
};

#endif // PLOCALESORTFILTERPROXYMODEL_H
