/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pRecursiveSortFilterProxyModel.h
** Date      : 2011-02-20T00:41:09
** License   : LGPL v3
** Home Page : http://bettercodes.org/projects/fresh
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
	\ingroup FreshGui
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
