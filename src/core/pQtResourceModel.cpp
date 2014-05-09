/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pQtResourceModel.cpp
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
#include "pQtResourceModel.h"

#if defined( QT_GUI_LIB )
#include "pGuiUtils.h"
#endif

#include <QDebug>

// Item

class pQtResourceModelItem
{
public:
  static QHash<QString, pQtResourceModelItem*> m_items;

public:
  pQtResourceModelItem( const QDir* dir, const QString& uri, int row, pQtResourceModelItem* parent = 0 ) {
    m_dir = dir;
    m_uri = uri;
    m_row = row;
    m_parent = parent;

    m_items[ m_uri ] = this;

    if ( m_uri == m_dir->path() ) {
      children();
    }
  }

  ~pQtResourceModelItem() {
    for ( int i = m_children.count() -1; i >= 0; i-- ) {
      pQtResourceModelItem* item = m_children.takeAt( i );
      m_items.remove( item->uri() );
      delete item;
    }
  }

  QList<pQtResourceModelItem*> children() {
    if ( m_children.isEmpty() ) {
      const QDir dir( m_uri );
      const QFileInfoList files = dir.entryInfoList( m_dir->nameFilters(), m_dir->filter() | QDir::AllDirs, m_dir->sorting() );

      for ( int i = 0; i < files.count(); i++ ) {
        const QFileInfo& file = files[ i ];
        m_children << new pQtResourceModelItem( m_dir, file.absoluteFilePath(), i, this );
      }
    }

    return m_children;
  }

  pQtResourceModelItem* child( int i ) {
    return children().value( i );
  }

  pQtResourceModelItem* parent() const {
    return m_parent;
  }

  QString uri() const {
    return m_uri;
  }

  int row() const {
    return m_row;
  }

private:
  const QDir* m_dir;
  QString m_uri;
  QList<pQtResourceModelItem*> m_children;
  pQtResourceModelItem* m_parent;
  int m_row;
};

QHash<QString, pQtResourceModelItem*> pQtResourceModelItem::m_items;

// Model

pQtResourceModel::pQtResourceModel( QObject* parent )
  : QAbstractItemModel( parent ),
    rootItem( 0 )
#if defined( QT_GUI_LIB )
    , imagesWildcards( pGuiUtils::supportedReadableImagesFormatsWildcards() )
#endif
{
  dir.setFilter( QDir::NoFilter );
  dir.setSorting( QDir::DirsFirst | QDir::LocaleAware );
  dir.setPath( QLatin1String( ":/" ) );

  resetModel();
}

pQtResourceModel::~pQtResourceModel()
{
  delete rootItem;
}

int pQtResourceModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED( parent );
  return 2;
}

QFileInfo pQtResourceModel::fileInfo( const QModelIndex& index ) const
{
  const pQtResourceModelItem* item = static_cast<pQtResourceModelItem*>( index.internalPointer() );
  return item ? QFileInfo( item->uri() ) : QFileInfo();
}

QModelIndex pQtResourceModel::modelIndex( const QString& filePath, int column ) const
{
  pQtResourceModelItem* item = pQtResourceModelItem::m_items.value( filePath );

  // the model is lazy populated, let try to discover parents items if needed
  if ( !item ) {
    const QFileInfo file( filePath );

    if ( file.exists() ) {
      const QString path = file.absolutePath();
      QDir d( path );

      while ( !item ) {
        item = pQtResourceModelItem::m_items.value( d.absolutePath() );

        if ( !item ) {
          if ( !d.cdUp() ) {
            break;
          }
        }
      }

      if ( item ) {
        item->children();

        if ( item->uri() == path ) {
          item = pQtResourceModelItem::m_items.value( filePath );
        }
        else {
          return modelIndex( filePath );
        }
      }
    }
  }

  return item
    ? createIndex( item->row(), column, item )
    : QModelIndex()
  ;
}

QModelIndexList pQtResourceModel::modelIndexList(const QStringList& filePaths, int column ) const
{
  QModelIndexList indexes;

  foreach ( const QString& filePath, filePaths ) {
    const QModelIndex index = modelIndex( filePath, column );

    if ( index.isValid() ) {
      indexes << index;
    }
  }

  return indexes;
}

QDir::Filters pQtResourceModel::filter() const
{
  return dir.filter();
}

void pQtResourceModel::setFilter( QDir::Filters filters )
{
  if ( dir.filter() != filters ) {
    dir.setFilter( filters );
    resetModel();
  }
}

QStringList pQtResourceModel::nameFilters() const
{
  return dir.nameFilters();
}

void pQtResourceModel::setNameFilters( const QStringList& nameFilters )
{
  if ( dir.nameFilters() != nameFilters ) {
    dir.setNameFilters( nameFilters );
    resetModel();
  }
}

QDir::SortFlags pQtResourceModel::sorting() const
{
  return dir.sorting();
}

void pQtResourceModel::setSorting( QDir::SortFlags sort )
{
  if ( dir.sorting() != sort ) {
    dir.setSorting( sort );
    resetModel();
  }
}

QString pQtResourceModel::path() const
{
  return dir.path();
}

void pQtResourceModel::setPath( const QString& path )
{
  if ( path.startsWith( QLatin1String( ":/" ) ) ) {
    dir.setPath( path );
    resetModel();
  }
}

void pQtResourceModel::resetModel()
{
  beginResetModel();
  delete rootItem;
  rootItem = new pQtResourceModelItem( &dir, dir.path(), 0 );
  endResetModel();
}

QVariant pQtResourceModel::data( const QModelIndex& index, int role ) const
{
  if ( !index.isValid() ) {
    return QVariant();
  }

  const QFileInfo file( fileInfo( index ) );

  switch ( role ) {
#if defined( QT_GUI_LIB )
    case Qt::DecorationRole: {
      return index.column() == 0
        ? ( QDir::match( imagesWildcards, file.fileName() ) ? QIcon( file.absoluteFilePath() ) : iconProvider.icon( file ) )
        : QVariant()
      ;
    }
#endif

    case Qt::DisplayRole: {
      switch ( index.column() ) {
        case 0:
          return file.fileName();
        case 1:
          return file.size();
      }
    }

    case Qt::ToolTipRole: {
      switch ( index.column() ) {
        case 0:
          return file.absoluteFilePath();
        case 1:
          return file.size();
      }
    }
  }

  return QVariant();
}

Qt::ItemFlags pQtResourceModel::flags( const QModelIndex& index ) const
{
  if ( !index.isValid() ) {
    return 0;
  }

  const QFileInfo fi( fileInfo( index ) );
  Qt::ItemFlags flags = Qt::ItemIsEnabled;

  if ( fi.isDir() && ( dir.filter().testFlag( QDir::Dirs ) || dir.filter().testFlag( QDir::AllDirs ) ) ) {
    flags |= Qt::ItemIsSelectable;
  }
  else if ( !fi.isDir() && dir.filter().testFlag( QDir::Files ) ) {
    flags |= Qt::ItemIsSelectable;
  }

  return flags;
}

QVariant pQtResourceModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if ( orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch ( section ) {
      case 0:
       return tr( "Name" );
      case 1:
       return tr( "Size" );
    }
  }

  return QAbstractItemModel::headerData( section, orientation, role );
}

QModelIndex pQtResourceModel::index( int row, int column, const QModelIndex& parent ) const
{
  if ( !hasIndex( row, column, parent ) ) {
    return QModelIndex();
  }

  pQtResourceModelItem* parentItem = 0;

  if ( parent == QModelIndex() ) {
    parentItem = rootItem;
  }
  else {
    parentItem = static_cast<pQtResourceModelItem*>( parent.internalPointer() );
  }

  if ( !parentItem ) {
    return QModelIndex();
  }

  pQtResourceModelItem* childItem = parentItem->child( row );
  return childItem ? createIndex( row, column, childItem ) : QModelIndex();
}

QModelIndex pQtResourceModel::parent( const QModelIndex& child ) const
{
  if ( !child.isValid() ) {
    return QModelIndex();
  }

  pQtResourceModelItem* childItem = static_cast<pQtResourceModelItem*>( child.internalPointer() );
  pQtResourceModelItem* parentItem = childItem->parent();

  if ( !parentItem || parentItem == rootItem ) {
    return QModelIndex();
  }

  return createIndex( parentItem->row(), 0, parentItem );
}

int pQtResourceModel::rowCount( const QModelIndex& parent ) const
{
  if ( parent.column() > 0 ) {
    return 0;
  }

  pQtResourceModelItem* parentItem;

  if ( parent == QModelIndex() ) {
    parentItem = rootItem;
  }
  else {
    parentItem = static_cast<pQtResourceModelItem*>( parent.internalPointer() );
  }

  return parentItem ? parentItem->children().count() : 0;
}
