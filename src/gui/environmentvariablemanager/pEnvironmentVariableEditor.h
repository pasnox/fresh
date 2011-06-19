/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pEnvironmentVariableEditor.h
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
#ifndef PENVIRONMENTVARIABLEEDITOR_H
#define PENVIRONMENTVARIABLEEDITOR_H

/*!
	\file pEnvironmentVariableEditor.h
	\brief Ths dialog allow you to create / edit a pEnvironmentVariablesModel::Variable.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pEnvironmentVariablesModel.h"

#include <QDialog>

class Ui_pEnvironmentVariableEditor;

/*!
	\ingroup FreshGui
	\class pEnvironmentVariableEditor
	\brief Ths dialog allow you to create / edit a pEnvironmentVariablesModel::Variable.
*/
class FRESH_EXPORT pEnvironmentVariableEditor : public QDialog 
{
	Q_OBJECT

public:
	/*!
		Create a new pEnvironmentVariableEditor having parent \a parent.
		If \a variable is not null then the name and value are read from it.
		If the variable name is not null then it will be read only.
	*/
	pEnvironmentVariableEditor( QWidget* parent = 0, const pEnvironmentVariablesModel::Variable& variable = pEnvironmentVariablesModel::Variable() );
	/*!
		Destroys the editor.
	*/
	virtual ~pEnvironmentVariableEditor();
	/*!
		Return the variable.
	*/
	pEnvironmentVariablesModel::Variable variable() const;

protected:
	Ui_pEnvironmentVariableEditor* ui;
};

#endif // PENVIRONMENTVARIABLEEDITOR_H
