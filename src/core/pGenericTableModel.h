/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pGenericTableModel.h
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
#ifndef PGENERICTABLEMODEL_H
#define PGENERICTABLEMODEL_H

/*!
    \file pGenericTableModel.h
    \brief A ready to use powerfull table model.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QAbstractTableModel>
#include <QPoint>

/*!
	\ingroup FreshCore
    \class pGenericTableModel
    \brief A ready to use powerfull table model.

    This model provide a powerfull ready to use table model. It has many usefull helper
    functions like swapping rows / columns, get checked indexes etc.
    The Qt::ItemFlags can be set using setData() with the pGenericTableModel::ItemFlagsRole role.
    The first usable user role is defined as pGenericTableModel::ExtendedUserRole.
*/
class FRESH_EXPORT pGenericTableModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    /*! This enum defines the extended roles the model. */
    enum ExtendedRole {
        ItemFlagsRole = Qt::UserRole, /*!< The role defining the index Qt::ItemFlags. */
        ExtendedUserRole = pGenericTableModel::ItemFlagsRole +1 /*!< The first usable user role. */
    };
    
    /*!
		Construct a pGenericTableModel having parent \a parent.
	*/
    pGenericTableModel( QObject* parent = 0 );
    /*!
		Reimplemented.
	*/
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    /*!
		Reimplemented.
	*/
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    /*!
		Reimplemented.
	*/
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    /*!
		Reimplemented.
	*/
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    /*!
		Reimplemented.
	*/
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    /*!
		Reimplemented.
	*/
    virtual bool setHeaderData( int section, Qt::Orientation orientation, const QVariant& value, int role = Qt::EditRole );
    /*!
		Reimplemented.
	*/
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    /*!
		Reimplemented.
	*/
    virtual void sort( int column, Qt::SortOrder order = Qt::AscendingOrder );
    /*!
		Reimplemented.
	*/
    virtual bool insertColumns( int column, int count, const QModelIndex& parent = QModelIndex() );
    /*!
		Reimplemented.
	*/
    virtual bool removeColumns( int column, int count, const QModelIndex& parent = QModelIndex() );
    /*!
		Reimplemented.
	*/
    virtual bool insertRows( int row, int count, const QModelIndex& parent = QModelIndex() );
    /*!
		Reimplemented.
	*/
    virtual bool removeRows( int row, int count, const QModelIndex& parent = QModelIndex() );
    /*!
		Set the column count to \a count.
	*/
    void setColumnCount( int count );
    /*!
		Set the row count to \a count.
	*/
    void setRowCount( int count );
    /*!
		Swap the columns from \a fromColumn to \a toColumn.
	*/
    bool swapColumns( int fromColumn, int toColumn );
    /*!
		Swap the rows from \a fromRow to \a toRow.
	*/
    bool swapRows( int fromRow, int toRow );
    /*!
		Clear the model data. If \a onlyData is true then only the data
        will be cleared, else the headers and layout will be cleared too.
	*/
    void clear( bool onlyData );
    /*!
		Return the list of all checked indexes from \a column.
        If \a column is -1 all the columns will be checked.
	*/
    QModelIndexList checkedIndexes( int column = -1 ) const;
    /*!
		Return the list of all checked rows from \a column.
        If \a column is -1 all the columns will be checked.
	*/
    QList<int> checkedRows( int column = -1 ) const;
    /*!
		Return the list of display role of all checked rows from \a column.
        If \a column is -1 all the columns will be checked.
	*/
    QStringList checkedStringList( int column = -1 ) const;
    /*!
		Clear all check state from indexes of \a column.
        If \a column is -1 all the columns will be checked.
	*/
    void clearCheckStates( int column = -1 );
    
protected:
    typedef QMap<int, QVariant> IntVariantMap; // represent an index
    typedef QMap<QPoint, pGenericTableModel::IntVariantMap> PointIntVariantMap; // represent an index data
    
    int mColumnCount;
    int mRowCount;
    pGenericTableModel::PointIntVariantMap mHeaderData;
    pGenericTableModel::PointIntVariantMap mData;
    
    pGenericTableModel::IntVariantMap* indexInternalData( const QModelIndex& index );
    pGenericTableModel::IntVariantMap indexInternalData( const QModelIndex& index ) const;
    
    pGenericTableModel::IntVariantMap* headerInternalData( int section, Qt::Orientation orientation );
    pGenericTableModel::IntVariantMap headerInternalData( int section, Qt::Orientation orientation ) const;
};

bool operator<( const QPoint& left, const QPoint& right );

Q_DECLARE_METATYPE( Qt::ItemFlags )

#endif // PGENERICTABLEMODEL_H
