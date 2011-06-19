/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
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

#include <QActionEvent>
#include <QDebug>

pActionsMenuBar::pActionsMenuBar( QWidget* parent )
	: QMenuBar( parent )
{
	mModel = 0;
}

pActionsMenuBar::~pActionsMenuBar()
{
}

void pActionsMenuBar::setModel( pActionsModel* model )
{
	if ( mModel ) {
		disconnect( mModel, SIGNAL( actionInserted( QAction* ) ), this, SLOT( model_actionInserted( QAction* ) ) );
		disconnect( mModel, SIGNAL( actionChanged( QAction* ) ), this, SLOT( model_actionChanged( QAction* ) ) );
		disconnect( mModel, SIGNAL( actionRemoved( QAction* ) ), this, SLOT( model_actionRemoved( QAction* ) ) );
		disconnect( mModel, SIGNAL( actionsCleared() ), this, SLOT( model_actionsCleared() ) );
		clear();
		mModel = 0;
	}
	
	mModel = model;
	
	if ( mModel ) {
		connect( mModel, SIGNAL( actionInserted( QAction* ) ), this, SLOT( model_actionInserted( QAction* ) ) );
		connect( mModel, SIGNAL( actionChanged( QAction* ) ), this, SLOT( model_actionChanged( QAction* ) ) );
		connect( mModel, SIGNAL( actionRemoved( QAction* ) ), this, SLOT( model_actionRemoved( QAction* ) ) );
		connect( mModel, SIGNAL( actionsCleared() ), this, SLOT( model_actionsCleared() ) );
		sync();
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

void pActionsMenuBar::recursiveSync( QAction* action )
{
	/*foreach ( const pActionsNode& childNode, node.children() ) {
		model_actionInserted( childNode );
		
		if ( childNode.hasChildren() ) {
			recursiveSync( childNode );
		}
	}*/
}

void pActionsMenuBar::sync()
{
	/*if ( !mModel ) {
		qDeleteAll( mMenus.values() );
		mMenus.clear();
		return;
	}
	
	recursiveSync( mModel->rootNode() );*/
}

void pActionsMenuBar::model_actionInserted( QAction* action )
{
	QAction* parent = mModel->parent( action );
	
	if ( !parent && action->menu() ) {
		addMenu( action->menu() );
	}
	
	/*QMenu* parentMenu = mMenus.value( node.path().section( '/', 0, -2 ) );
	
	switch ( node.type() ) {
		case pActionsNode::Action: {
			parentMenu->addAction( node.action() );
			break;
		}
		case pActionsNode::Path: {
			QMenu* menu = new QMenu;
			
			menu->setObjectName( node.path() );
			mMenus[ node.path() ] = menu;
			
			if ( parentMenu ) {
				parentMenu->addMenu( menu );
			}
			else {
				QMenuBar::addMenu( menu );
			}
			
			model_actionChanged( node );
			
			break;
		}
		default:
			break;
	}*/
}

void pActionsMenuBar::model_actionChanged( QAction* action )
{
	/*switch ( node.type() ) {
		case pActionsNode::Action:
			break;
		case pActionsNode::Path: {
			QMenu* menu = mMenus.value( node.path() );
			
			menu->setIcon( node.icon() );
			menu->setTitle( node.text() );
		}
		default:
			break;
	}*/
}

void pActionsMenuBar::model_actionRemoved( QAction* action )
{
	/*QAction* parent = mModel->parent( action );
	
	if ( !parent && action->menu() ) {
	qWarning() << "removing menu" << action->menu() << action->menu()->title();
		removeAction( action );
	}*/
	
	/*if ( node.type() == pActionsNode::Path ) {
		QMenu* menu = mMenus.take( node.path() );
		menu->deleteLater();
	}
	else {
		QMenu* parentMenu = mMenus.value( node.path().section( '/', 0, -2 ) );
		parentMenu->removeAction( node.action() );
	}*/
}

void pActionsMenuBar::model_actionsCleared()
{
	clear();
}
