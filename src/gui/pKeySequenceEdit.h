/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pKeySequenceEdit.h
** Date      : 2011-02-20T00:41:35
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
#ifndef PKEYSEQUENCEEDIT_H
#define PKEYSEQUENCEEDIT_H

/*!
	\file pKeySequenceEdit.h
	\brief A special QLineEdit that allow to defin QShortcut.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pLineEdit.h"

/*!
	\ingroup Gui
	\class pKeySequenceEdit
	\brief A special QLineEdit that allow to define QShortcut.

	When the user press some combinaison keys, the result is wrotten in the pLineEdit.
*/
class FRESH_EXPORT pKeySequenceEdit : public pLineEdit
{
	Q_OBJECT

public:
	/*!
		Create a instance of pKeySequenceEdit having parent \a parent.
	*/
	pKeySequenceEdit( QWidget* parent = 0 );
	/*!
		Create a instance of pKeySequenceEdit having parent \a parent.
		The widget text is set to \a content.
	*/
	pKeySequenceEdit( const QString& contents, QWidget* parent = 0 );

protected:
	mutable bool mFinished;
	
	/*!
		Reimplemented.
	*/
	virtual void keyPressEvent( QKeyEvent* event );
	/*!
		Reimplemented.
	*/
	virtual void keyReleaseEvent( QKeyEvent* event );
	
	QString keySequence( QKeyEvent* event ) const;
};

#endif // PKEYSEQUENCEEDIT_H
