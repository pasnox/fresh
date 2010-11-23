#include "pEnvironmentVariableEditor.h"
#include "ui_pEnvironmentVariableEditor.h"

pEnvironmentVariableEditor::pEnvironmentVariableEditor( QWidget* parent, const QString& name, const QString& value )
	: QDialog( parent )
{
	ui = new Ui_pEnvironmentVariableEditor;
	
	ui->setupUi( this );
	ui->leName->setReadOnly( !name.isEmpty() );
	ui->leName->setText( name );
	ui->leValue->setText( value );

	if ( name.isEmpty() ) {
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
