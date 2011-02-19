/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pEnvironmentVariablesEditor.h
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
#ifndef PENVIRONMENTVARIABLESEDITOR_H
#define PENVIRONMENTVARIABLESEDITOR_H

/*!
	\file pEnvironmentVariablesEditor.h
	\brief A pEnvironmentVariablesModel::Variable list widget editor.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pEnvironmentVariablesModel.h"

#include <QWidget>

class Ui_pEnvironmentVariablesEditor;

/*!
	\ingroup Gui
	\class pEnvironmentVariablesEditor
	\brief A pEnvironmentVariablesModel::Variable list widget editor.
*/
class FRESH_EXPORT pEnvironmentVariablesEditor : public QWidget
{
	Q_OBJECT

public:
	/*!
		Create a variables editor having \a parent as parent.
	*/
	pEnvironmentVariablesEditor( QWidget* parent = 0 );
	/*!
		Destroys the editor.
	*/
	virtual ~pEnvironmentVariablesEditor();
	/*!
		Return all variables.
	*/
	pEnvironmentVariablesModel::Variables variables() const;
	/*!
		Return all the default variables.
	*/
	pEnvironmentVariablesModel::Variables defaultVariables() const;
	/*!
		Return a string list containing pairs of key=value variables.
		If \a keepDisabled is false then only enabled variable will be returned else all.
	*/
	QStringList variables( bool keepDisabled ) const;
	/*!
		Return a variable by its name.
	*/
	pEnvironmentVariablesModel::Variable variable( const QString& name ) const;
	/*!
		Return true if the variable \a name exists else false.
	*/
	bool contains( const QString& name ) const;
	/*!
		Return true if the editor is empty (no variables defined) else false.
	*/
	bool isEmpty() const;

public slots:
	/*!
		Set the variables to edit to \a variables making them the default ones if \a setDefault is true.
		If \a setDefault is false then the default variables is not changed.
	*/
	void setVariables( const pEnvironmentVariablesModel::Variables& variables, bool setDefault );
	/*!
		Set the default variables to \a variables.
	*/
	void setDefaultVariables( const pEnvironmentVariablesModel::Variables& variables );
	/*!
		Set the variables to edit to \a variables making them the default ones if \a setDefault is true.
		If \a setDefault is false then the default variables is not changed.
		The variables must be a list of pairs of key=value.
	*/
	void setVariables( const QStringList& variables, bool setDefault );
	/*!
		Replace the variable named \a name by \a variable.
	*/
	void setVariable( const QString& name, const pEnvironmentVariablesModel::Variable& variable );
	/*!
		Remove the variable named \a name.
	*/
	void removeVariable( const QString& name );
	/*!
		Clear all variables.
	*/
	void clearVariables();
	/*!
		Set the variables t obe the default variables.
	*/
	void resetVariablesToDefault();
	/*!
		Set the variables t obe all ones defines in QProcess::systemEnvironment()
		making them default according to \a setDefault.
		If \a setDefault is false then the default variables is not changed.
	*/
	void resetVariablesToSystem( bool setDefault );

protected:
	Ui_pEnvironmentVariablesEditor* ui;
	pEnvironmentVariablesModel* mModel;

protected slots:
	void model_view_changed();
	void on_aAdd_triggered();
	void on_aEdit_triggered();
	void on_aRemove_triggered();
	void on_aClear_triggered();
	void on_aResetDefault_triggered();
	void on_aResetSystem_triggered();
	void on_tvVariables_activated( const QModelIndex& index );
};

#endif // PENVIRONMENTVARIABLESEDITOR_H
