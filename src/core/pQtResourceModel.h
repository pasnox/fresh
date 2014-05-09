/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pQtResourceModel.h
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
#ifndef PQTRESOURCEMODEL_H
#define PQTRESOURCEMODEL_H

/*!
    \file pQtResourceModel.h
    \brief A model presenting QtResource.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QAbstractItemModel>
#if defined( QT_GUI_LIB )
#include <QFileIconProvider>
#endif
#include <QDir>

class pQtResourceModelItem;

// TODO: Finish documentation

/*!
    \ingroup FreshCore
    \class pQtResourceModel
    \brief A model presenting QtResource.
*/
class FRESH_EXPORT pQtResourceModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  pQtResourceModel( QObject* parent = 0 );
  ~pQtResourceModel();

  virtual QVariant data( const QModelIndex& index, int role ) const;
  virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
  virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
  virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
  virtual QModelIndex parent(const QModelIndex& child ) const;
  virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
  virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;

  QFileInfo fileInfo( const QModelIndex& index ) const;
  QModelIndex modelIndex( const QString& filePath, int column = 0 ) const;
  QModelIndexList modelIndexList( const QStringList& filePaths, int column = 0 ) const;

  QDir::Filters filter() const;
  void setFilter( QDir::Filters filters );

  QStringList nameFilters() const;
  void setNameFilters( const QStringList& nameFilters );

  QDir::SortFlags sorting() const;
  void setSorting( QDir::SortFlags sort );

  QString path() const;
  void setPath( const QString& path );

private:
  pQtResourceModelItem* rootItem;
#if defined( QT_GUI_LIB )
  QFileIconProvider iconProvider;
  QStringList imagesWildcards;
#endif
  QDir dir;

  void resetModel();
};

#endif // PQTRESOURCEMODEL_H
