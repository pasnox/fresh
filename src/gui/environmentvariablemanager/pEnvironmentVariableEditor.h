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
	\ingroup Gui
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
