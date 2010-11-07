#ifndef PRECURSIVESORTFILTERPROXYMODEL_H
#define PRECURSIVESORTFILTERPROXYMODEL_H

#include "core/FreshExport.h"

#include <QSortFilterProxyModel>

class FRESH_EXPORT pRecursiveSortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
	
public:
	pRecursiveSortFilterProxyModel( QObject* parent = 0 );

protected:
	virtual bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const;
};

#endif // PRECURSIVESORTFILTERPROXYMODEL_H
