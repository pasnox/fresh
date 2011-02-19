/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pEnvironmentVariablesEditor.cpp
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
#include "pEnvironmentVariablesEditor.h"
#include "ui_pEnvironmentVariablesEditor.h"
#include "pEnvironmentVariableEditor.h"

#include <QMessageBox>

pEnvironmentVariablesEditor::pEnvironmentVariablesEditor( QWidget* parent )
	: QWidget( parent )
{
	ui = new Ui_pEnvironmentVariablesEditor;
	
	ui->setupUi( this );
	ui->verticalLayout->setMenuBar( ui->tbActions );

	mModel = new pEnvironmentVariablesModel( this );
	ui->tvVariables->setModel( mModel );
	ui->tvVariables->header()->setResizeMode( 0, QHeaderView::ResizeToContents );

	model_view_changed();

	connect( mModel, SIGNAL( defaultVariablesChanged() ), this, SLOT( model_view_changed() ) );
	connect( mModel, SIGNAL( rowsInserted( const QModelIndex&, int, int ) ), this, SLOT( model_view_changed() ) );
	connect( mModel, SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ), this, SLOT( model_view_changed() ) );
	connect( mModel, SIGNAL( layoutChanged() ), this, SLOT( model_view_changed() ) );
	connect( ui->tvVariables->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( model_view_changed() ) );
}

pEnvironmentVariablesEditor::~pEnvironmentVariablesEditor()
{
	delete ui;
}

pEnvironmentVariablesModel::Variables pEnvironmentVariablesEditor::variables() const
{
	return mModel->variables();
}

pEnvironmentVariablesModel::Variables pEnvironmentVariablesEditor::defaultVariables() const
{
	return mModel->defaultVariables();
}

QStringList pEnvironmentVariablesEditor::variables( bool keepDisabled ) const
{
	return mModel->variables( keepDisabled );
}

pEnvironmentVariablesModel::Variable pEnvironmentVariablesEditor::variable( const QString& name ) const
{
	return mModel->variable( name );
}

bool pEnvironmentVariablesEditor::contains( const QString& name ) const
{
	return mModel->contains( name );
}

bool pEnvironmentVariablesEditor::isEmpty() const
{
	return mModel->isEmpty();
}

void pEnvironmentVariablesEditor::setVariables( const pEnvironmentVariablesModel::Variables& variables, bool setDefault )
{
	mModel->setVariables( variables, setDefault );
}

void pEnvironmentVariablesEditor::setDefaultVariables( const pEnvironmentVariablesModel::Variables& variables )
{
	mModel->setDefaultVariables( variables );
}

void pEnvironmentVariablesEditor::setVariables( const QStringList& variables, bool setDefault )
{
	mModel->setVariables( variables, setDefault );
}

void pEnvironmentVariablesEditor::setVariable( const QString& name, const pEnvironmentVariablesModel::Variable& variable )
{
	mModel->setVariable( name, variable );
}

void pEnvironmentVariablesEditor::removeVariable( const QString& name )
{
	mModel->removeVariable( name );
}

void pEnvironmentVariablesEditor::clearVariables()
{
	mModel->clearVariables();
}

void pEnvironmentVariablesEditor::resetVariablesToDefault()
{
	mModel->resetVariablesToDefault();
}

void pEnvironmentVariablesEditor::resetVariablesToSystem( bool setDefault )
{
	mModel->resetVariablesToSystem( setDefault );
}

void pEnvironmentVariablesEditor::model_view_changed()
{
	const bool hasSelection = ui->tvVariables->selectionModel()->hasSelection();

	ui->aEdit->setEnabled( hasSelection );
	ui->aRemove->setEnabled( hasSelection );
	ui->aClear->setEnabled( mModel->hasChildren() );
	ui->aResetDefault->setEnabled( !mModel->defaultVariables().isEmpty() );
}

void pEnvironmentVariablesEditor::on_aAdd_triggered()
{
	pEnvironmentVariableEditor dlg( this );
	dlg.setWindowTitle( tr( "Add a new variable..." ) );

	if ( dlg.exec() == QDialog::Rejected ) {
		return;
	}

	pEnvironmentVariablesModel::Variable variable = dlg.variable();
	
	if ( variable.name.isEmpty() ) {
		return;
	}

	if ( mModel->contains( variable.name ) ) {
		const QMessageBox::StandardButton result = QMessageBox::question( this, QString::null,
			tr( "The variable '%1' already exists, update it?" ).arg( variable.name ),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );

		if ( result != QMessageBox::Yes ) {
			return;
		}
		
		variable = mModel->variable( variable.name );
	}

	variable.value = dlg.variable().value;
	mModel->setVariable( variable.name, variable );

	const QModelIndex index = mModel->index( variable.name );
	ui->tvVariables->setCurrentIndex( index );
}

void pEnvironmentVariablesEditor::on_aEdit_triggered()
{
	const QModelIndex index = ui->tvVariables->selectionModel()->selectedIndexes().value( 0 );
	pEnvironmentVariablesModel::Variable variable = mModel->variable( index );

	pEnvironmentVariableEditor dlg( this, variable );
	dlg.setWindowTitle( tr( "Edit a variable..." ) );

	if ( dlg.exec() == QDialog::Rejected ) {
		return;
	}

	variable.value = dlg.variable().value;
	mModel->setVariable( variable.name, variable );
}

void pEnvironmentVariablesEditor::on_aRemove_triggered()
{
	const QModelIndex index = ui->tvVariables->selectionModel()->selectedIndexes().value( 0 );
	const pEnvironmentVariablesModel::Variable variable = mModel->variable( index );
	const QMessageBox::StandardButton result = QMessageBox::question( this, QString::null,
		tr( "Are you sure you want to remove the variable '%1' ?" ).arg( variable.name ),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );

	if ( result == QMessageBox::No ) {
		return;
	}

	mModel->removeVariable( variable.name );
}

void pEnvironmentVariablesEditor::on_aClear_triggered()
{
	const QMessageBox::StandardButton result = QMessageBox::question( this, QString::null,
		tr( "Are you sure you want to clear all variables?" ),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );

	if ( result == QMessageBox::No ) {
		return;
	}

	mModel->clearVariables();
}

void pEnvironmentVariablesEditor::on_aResetDefault_triggered()
{
	mModel->resetVariablesToDefault();
}

void pEnvironmentVariablesEditor::on_aResetSystem_triggered()
{
	mModel->resetVariablesToSystem( false );
}

void pEnvironmentVariablesEditor::on_tvVariables_activated( const QModelIndex& index )
{
	Q_UNUSED( index );
	
	if ( index.column() == 1 ) {
		on_aEdit_triggered();
	}
}
