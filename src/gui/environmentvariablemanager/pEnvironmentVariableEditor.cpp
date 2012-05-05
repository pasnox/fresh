/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pEnvironmentVariableEditor.cpp
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
