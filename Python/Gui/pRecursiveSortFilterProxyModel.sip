class pRecursiveSortFilterProxyModel : QSortFilterProxyModel
{
%TypeHeaderCode
#include <../src/gui/models/pRecursiveSortFilterProxyModel.h>
%End

public:
	pRecursiveSortFilterProxyModel( QObject* parent = 0 );

protected:
	virtual bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const;
};
