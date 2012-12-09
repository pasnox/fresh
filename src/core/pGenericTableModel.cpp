/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pGenericTableModel.cpp
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
#include "pGenericTableModel.h"

#include <QStringList>
#include <QDebug>

bool operator<( const QPoint& left, const QPoint& right )
{
    return qMakePair( left.x(), left.y() ) < qMakePair( right.x(), right.y() );
}

pGenericTableModel::pGenericTableModel( QObject* parent )
    : QAbstractTableModel( parent )
{
    mColumnCount = 0;
    mRowCount = 0;
}

int pGenericTableModel::columnCount( const QModelIndex& parent ) const
{
    return parent == QModelIndex() ? mColumnCount : 0;
}

int pGenericTableModel::rowCount( const QModelIndex& parent ) const
{
    return parent == QModelIndex() ? mRowCount : 0;
}

QVariant pGenericTableModel::data( const QModelIndex& index, int role ) const
{
    switch ( role ) {
        case pGenericTableModel::ItemFlagsRole:
            return QVariant::fromValue( flags( index ) );
        default: {
            QVariant value = indexInternalData( index ).value( role );
            
            if ( value.isNull() && role == Qt::DisplayRole ) {
                value = indexInternalData( index ).value( Qt::EditRole );
            }
            
            return value;
        }
    }
}

QVariant pGenericTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    return headerInternalData( section, orientation ).value( role );
}

bool pGenericTableModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    pGenericTableModel::IntVariantMap* map = indexInternalData( index );
    
    if ( !map ) {
        return false;
    }
    
    if ( value.isNull() ) {
        if ( !map->contains( role ) ) {
            return true;
        }
        
        map->remove( role );
    }
    else {
        if ( map->value( role ) == value ) {
            return true;
        }
        
        (*map)[ role ] = value;
    }
    
    emit dataChanged( index, index );
    return true;
}

bool pGenericTableModel::setHeaderData( int section, Qt::Orientation orientation, const QVariant& value, int role )
{
    pGenericTableModel::IntVariantMap* map = headerInternalData( section, orientation );
    
    if ( !map ) {
        return false;
    }
    
    (*map)[ role ] = value;
    emit headerDataChanged( orientation, section, section );
    return true;
}

Qt::ItemFlags pGenericTableModel::flags( const QModelIndex& index ) const
{
    const QVariant variant = indexInternalData( index ).value( pGenericTableModel::ItemFlagsRole );
    return variant.isNull() ? QAbstractTableModel::flags( index ) : variant.value<Qt::ItemFlags>();
}

void pGenericTableModel::sort( int column, Qt::SortOrder order )
{
    Q_UNUSED( column );
    Q_UNUSED( order );
    qWarning() << Q_FUNC_INFO << "Not yet implemented";
}

bool pGenericTableModel::insertColumns( int column, int count, const QModelIndex& parent )
{
    Q_UNUSED( column );
    Q_UNUSED( count );
    Q_UNUSED( parent );
    qWarning() << Q_FUNC_INFO << "Not yet implemented";
    return false;
}

bool pGenericTableModel::removeColumns( int column, int count, const QModelIndex& parent )
{
    Q_UNUSED( column );
    Q_UNUSED( count );
    Q_UNUSED( parent );
    qWarning() << Q_FUNC_INFO << "Not yet implemented";
    return false;
}

bool pGenericTableModel::insertRows( int row, int count, const QModelIndex& parent )
{
    if ( parent != QModelIndex() || row < 0 || row > mRowCount || count <= 0 || mColumnCount == 0 ) {
        return false;
    }
    
    beginInsertRows( parent, row, row +count -1 );
    mRowCount += count;
    const QList<QPoint> keys = mData.keys();
    for ( int i = mData.count() -1; i >= 0; i-- ) {
        const QPoint& point = keys[ i ];
        
        if ( point.y() >= row ) {
            mData[ QPoint( point.x(), point.y() +count ) ] = mData.take( point );
        }
    }
    endInsertRows();
    return true;
}

bool pGenericTableModel::removeRows( int row, int count, const QModelIndex& parent )
{
    if ( parent != QModelIndex() || row < 0 || row >= mRowCount || row +count -1 >= mRowCount ) {
        return false;
    }
    
    beginRemoveRows( parent, row, row +count -1 );
    mRowCount -= count;
    foreach ( const QPoint& point, mData.keys() ) {
        if ( point.y() >= row && point.y() < row +count ) {
            mData.remove( point );
        }
        else if ( point.y() >= row +count ) {
            mData[ QPoint( point.x(), point.y() -count ) ] = mData.take( point );
        }
    }
    endRemoveRows();
    return true;
}

void pGenericTableModel::setColumnCount( int count )
{
    if ( count == mColumnCount ) {
        return;
    }
    
    if ( count > mColumnCount ) {
        const int diff = count -mColumnCount;
        beginInsertColumns( QModelIndex(), mColumnCount, diff -1 );
        mColumnCount = count;
        endInsertColumns();
    }
    else {
        beginRemoveColumns( QModelIndex(), count, mColumnCount -1 );
        mColumnCount = count;
        foreach ( const QPoint& point, mData.keys() ) {
            if ( point.x() >= mColumnCount ) {
                mData.remove( point );
            }
        }
        foreach ( const QPoint& point, mHeaderData.keys() ) {
            if ( point.x() == Qt::Horizontal && point.y() >= mColumnCount ) {
                mHeaderData.remove( point );
            }
        }
        endRemoveColumns();
    }
}

void pGenericTableModel::setRowCount( int count )
{
    if ( count == mRowCount ) {
        return;
    }
    
    if ( count > mRowCount ) {
        beginInsertRows( QModelIndex(), mRowCount, count -1 );
        mRowCount = count;
        endInsertRows();
    }
    else {
        beginRemoveRows( QModelIndex(), count, mRowCount -1 );
        mRowCount = count;
        foreach ( const QPoint& point, mData.keys() ) {
            if ( point.y() >= mRowCount ) {
                mData.remove( point );
            }
        }
        foreach ( const QPoint& point, mHeaderData.keys() ) {
            if ( point.x() == Qt::Vertical && point.y() >= mRowCount ) {
                mHeaderData.remove( point );
            }
        }
        endRemoveRows();
    }
}

bool pGenericTableModel::swapColumns( int fromColumn, int toColumn )
{
    Q_UNUSED( fromColumn );
    Q_UNUSED( toColumn );
    qWarning() << Q_FUNC_INFO << "Not yet implemented";
    return false;
}

bool pGenericTableModel::swapRows( int fromRow, int toRow )
{
    if ( fromRow < 0 || fromRow >= mRowCount || toRow < 0 || toRow >= mRowCount ) {
        return false;
    }
    
    if ( fromRow == toRow ) {
        return true;
    }
    
    emit layoutAboutToBeChanged();
    
    const QModelIndexList oldIndexes = persistentIndexList();
    QModelIndexList newIndexes;
    pGenericTableModel::PointIntVariantMap fromData;
    pGenericTableModel::PointIntVariantMap toData;
    
    // keep backup data
    foreach( const QPoint& point, mData.keys() ) {
        if ( point.y() == fromRow ) {
            fromData[ point ] = mData.take( point );
        }
        else if ( point.y() == toRow ) {
            toData[ point ] = mData.take( point );
        }
    }
    
    // move from data
    foreach( const QPoint& point, fromData.keys() ) {
        mData[ QPoint( point.x(), toRow ) ] = fromData.take( point );
    }
    
    // move to data
    foreach( const QPoint& point, toData.keys() ) {
        mData[ QPoint( point.x(), fromRow ) ] = toData.take( point );
    }
    
    // update persistent indexes
    foreach ( const QModelIndex& index, oldIndexes ) {
        if ( index.row() == fromRow ) {
            newIndexes << this->index( toRow, index.column(), index.parent() );
        }
        else if ( index.row() == toRow ) {
            newIndexes << this->index( fromRow, index.column(), index.parent() );
        }
        else {
            newIndexes << index;
        }
    }
    
    changePersistentIndexList( oldIndexes, newIndexes );
    
    emit layoutChanged();
    
    return true;
}

pGenericTableModel::IntVariantMap* pGenericTableModel::indexInternalData( const QModelIndex& index )
{
    return index.isValid() ? &mData[ QPoint( index.column(), index.row() ) ] : 0;
}

pGenericTableModel::IntVariantMap pGenericTableModel::indexInternalData( const QModelIndex& index ) const
{
    return mData.value( QPoint( index.column(), index.row() ) );
}

pGenericTableModel::IntVariantMap* pGenericTableModel::headerInternalData( int section, Qt::Orientation orientation )
{
    switch ( orientation ) {
        case Qt::Horizontal:
            return section >= 0 && section < mColumnCount ? &mHeaderData[ QPoint( (int)orientation, section ) ] : 0;
        case Qt::Vertical:
            return section >= 0 && section < mRowCount ? &mHeaderData[ QPoint( (int)orientation, section ) ] : 0;
    }
    
    return 0;
}

pGenericTableModel::IntVariantMap pGenericTableModel::headerInternalData( int section, Qt::Orientation orientation ) const
{
    return mHeaderData.value( QPoint( (int)orientation, section ) );
}

void pGenericTableModel::clear( bool onlyData )
{
    if ( rowCount() == 0 && columnCount() == 0 ) {
        return;
    }
    
    if ( onlyData ) {
        beginResetModel();
        mData.clear();
        endResetModel();
    }
    else {
        beginRemoveColumns( QModelIndex(), 0, mColumnCount -1 );
        mColumnCount = 0;
        mHeaderData.clear();
        endRemoveColumns();
        
        beginRemoveRows( QModelIndex(), 0, mRowCount -1 );
        mRowCount = 0;
        mData.clear();
        endRemoveRows();
    }
}

QModelIndexList pGenericTableModel::checkedIndexes( int column ) const
{
    QModelIndexList indexes;
    
    foreach ( const QPoint& point, mData.keys() ) {
        if ( column != -1 && column != point.x() ) {
            continue;
        }
        
        const pGenericTableModel::IntVariantMap& map = mData[ point ];
        const QVariant value = map.value( Qt::CheckStateRole );
        const Qt::CheckState state = value.isNull() ? Qt::Unchecked : Qt::CheckState( value.toInt() );
        
        if ( state == Qt::Checked ) {
            indexes << index( point.y(), point.x() );
        }
    }
    
    return indexes;
}

QList<int> pGenericTableModel::checkedRows( int column ) const
{
    QList<int> rows;
    
    foreach ( const QPoint& point, mData.keys() ) {
        if ( column != -1 && column != point.x() ) {
            continue;
        }
        
        const pGenericTableModel::IntVariantMap& map = mData[ point ];
        const QVariant value = map.value( Qt::CheckStateRole );
        const Qt::CheckState state = value.isNull() ? Qt::Unchecked : Qt::CheckState( value.toInt() );
        
        if ( state == Qt::Checked ) {
            rows << point.y();
        }
    }
    
    return rows;
}

QStringList pGenericTableModel::checkedStringList( int column ) const
{
    QStringList strings;
    
    foreach ( const QPoint& point, mData.keys() ) {
        if ( column != -1 && column != point.x() ) {
            continue;
        }
        
        const pGenericTableModel::IntVariantMap& map = mData[ point ];
        const QVariant value = map.value( Qt::CheckStateRole );
        const Qt::CheckState state = value.isNull() ? Qt::Unchecked : Qt::CheckState( value.toInt() );
        
        if ( state == Qt::Checked ) {
            QVariant value = map.value( Qt::DisplayRole );
            
            if ( value.isNull() ) {
                value = map.value( Qt::EditRole );
            }
            
            strings << value.toString();
        }
    }
    
    return strings;
}

void pGenericTableModel::clearCheckStates( int column )
{
    if ( rowCount() == 0 ) {
        return;
    }
    
    foreach ( const QPoint& point, mData.keys() ) {
        if ( column != -1 && column != point.x() ) {
            continue;
        }
        
        pGenericTableModel::IntVariantMap& map = mData[ point ];
        map.remove( Qt::CheckStateRole );
    }
    
    emit dataChanged( index( 0, 0 ), index( rowCount() -1, columnCount() -1 ) );
}
