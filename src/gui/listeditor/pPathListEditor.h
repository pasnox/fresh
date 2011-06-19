/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pPathListEditor.h
** Date      : 2011-02-20T00:41:09
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
#ifndef PPATHLISTEDITOR_H
#define PPATHLISTEDITOR_H

/*!
	\file pPathListEditor.h
	\brief a pStringListEditor that handle paths name.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pFileListEditor.h"

/*!
	\ingroup FreshGui
	\class pPathListEditor
	\brief a pStringListEditor that handle paths name.
*/
class FRESH_EXPORT pPathListEditor : public pFileListEditor
{
	Q_OBJECT
	
public:
	/*!
		Create an editor instance having parent \a parent.
	*/
	pPathListEditor( QWidget* parent = 0 );
	/*!
		Create an editor instance having parent \a parent and title \a title.
		The default path for the file dialog will be \a path.
	*/
	pPathListEditor( const QString& title, const QString& path, QWidget* parent = 0 );

protected:
	virtual void init();
	
protected slots:
	virtual void onAddItem();
	virtual void onEditItem();
};

#endif // PPATHLISTEDITOR_H
