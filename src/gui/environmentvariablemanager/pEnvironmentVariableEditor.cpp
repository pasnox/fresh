#include "pEnvironmentVariableEditor.h"
#include "ui_pEnvironmentVariableEditor.h"

pEnvironmentVariableEditor::pEnvironmentVariableEditor( QWidget* parent, const pEnvironmentVariablesModel::Variable& variable )
	: QDialog( parent )
{
	ui = new Ui_pEnvironmentVariableEditor;
	
	ui->setupUi( this );
	ui->leName->setReadOnly( !variable.name.isEmpty() );
	ui->leName->setText( variable.name );
	ui->leValue->setText( variable.value );

	if ( variable.name.isEmpty() ) {
		ui->leName->setFocus();
	}
	else {
		ui->leValue->setFocus();
	}
}

pEnvironmentVariableEditor::~pEnvironmentVariableEditor()
{
	delete ui;
}

pEnvironmentVariablesModel::Variable pEnvironmentVariableEditor::variable() const
{
	return pEnvironmentVariablesModel::Variable( ui->leName->text(), ui->leValue->text() );
}
