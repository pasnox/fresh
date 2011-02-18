#ifndef PRECURSIVESORTFILTERPROXYMODEL_H
#define PRECURSIVESORTFILTERPROXYMODEL_H

/*!
	\file pRecursiveSortFilterProxyModel.h
	\brief A QSortFilterProxyModel that does the rows filtering recursively.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QSortFilterProxyModel>

/*!
	\ingroup Gui
	\class pRecursiveSortFilterProxyModel
	\brief A QSortFilterProxyModel that does the rows filtering recursively.

	The standard QSortFilterProxyModel does not do recursive filtering.
	What is called recursive if that if a node does not match but one of its children match,
	then the parent node will be filtered out making the child not visible.
	This model recursively check for children and does not filter out parent having matching children.
*/
class FRESH_EXPORT pRecursiveSortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
	
public:
	/*!
		Create an instance of the filter model having parent \a parent.
	*/
	pRecursiveSortFilterProxyModel( QObject* parent = 0 );

protected:
	/*!
		Reimplemented
	*/
	virtual bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const;
};

#endif // PRECURSIVESORTFILTERPROXYMODEL_H
