/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsMenuBar.cpp
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
#include "pActionsMenuBar.h"
#include "pActionsModel.h"

pActionsMenuBar::pActionsMenuBar( QWidget* parent )
    : QMenuBar( parent )
{
    mModel = 0;
}

void pActionsMenuBar::setModel( pActionsModel* model )
{
    if ( mModel ) {
        disconnect( mModel, SIGNAL( actionInserted( QAction* ) ), this, SLOT( model_actionInserted( QAction* ) ) );
        disconnect( mModel, SIGNAL( actionsCleared() ), this, SLOT( model_actionsCleared() ) );
        clear();
        mModel = 0;
    }
    
    mModel = model;
    
    if ( mModel ) {
        for ( int i = 0; i < mModel->rowCount(); i++ ) {
            QAction* action = mModel->action( mModel->index( i, 0 ) );
            model_actionInserted( action );
        }
    }
    
    if ( mModel ) {
        connect( mModel, SIGNAL( actionInserted( QAction* ) ), this, SLOT( model_actionInserted( QAction* ) ) );
        connect( mModel, SIGNAL( actionsCleared() ), this, SLOT( model_actionsCleared() ) );
    }
}

pActionsModel* pActionsMenuBar::model() const
{
    if ( !mModel ) {
        pActionsMenuBar* mb = const_cast<pActionsMenuBar*>( this );
        mb->setModel( new pActionsModel( mb ) );
    }
    
    return mModel;
}

void pActionsMenuBar::model_actionInserted( QAction* action )
{
    QAction* parent = mModel->parent( action );
    
    if ( !parent && action->menu() ) {
        addMenu( action->menu() );
    }
}

void pActionsMenuBar::model_actionsCleared()
{
    clear();
}
