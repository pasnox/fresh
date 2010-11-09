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
