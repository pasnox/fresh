/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsNodeMenuBar.cpp
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
#include "pActionsNodeMenuBar.h"
#include "pActionsNodeModel.h"

#include <QEvent>
#include <QActionEvent>
#include <QToolButton>

pActionsNodeMenuBar::pActionsNodeMenuBar( QWidget* parent )
	: QMenuBar( parent )
{
	mModel = 0;
}

pActionsNodeMenuBar::~pActionsNodeMenuBar()
{
	if ( mModel ) {
		mModel->clear();
	}
}

void pActionsNodeMenuBar::setModel( pActionsNodeModel* model )
{
	if ( mModel ) {
		mModel->clear();
		disconnect( mModel, SIGNAL( nodeInserted( const pActionsNode& ) ), this, SLOT( model_nodeInserted( const pActionsNode& ) ) );
		disconnect( mModel, SIGNAL( nodeChanged( const pActionsNode& ) ), this, SLOT( model_nodeChanged( const pActionsNode& ) ) );
		disconnect( mModel, SIGNAL( nodeRemoved( const pActionsNode& ) ), this, SLOT( model_nodeRemoved( const pActionsNode& ) ) );
		disconnect( mModel, SIGNAL( nodesCleared() ), this, SLOT( model_nodesCleared() ) );
		mModel = 0;
	}
	
	mModel = model;
	
	if ( mModel ) {
		connect( mModel, SIGNAL( nodeInserted( const pActionsNode& ) ), this, SLOT( model_nodeInserted( const pActionsNode& ) ) );
		connect( mModel, SIGNAL( nodeChanged( const pActionsNode& ) ), this, SLOT( model_nodeChanged( const pActionsNode& ) ) );
		connect( mModel, SIGNAL( nodeRemoved( const pActionsNode& ) ), this, SLOT( model_nodeRemoved( const pActionsNode& ) ) );
		connect( mModel, SIGNAL( nodesCleared() ), this, SLOT( model_nodesCleared() ) );
		sync();
	}
}

pActionsNodeModel* pActionsNodeMenuBar::model() const
{
	return mModel;
}

bool pActionsNodeMenuBar::addAction( const QString& path, QAction* action )
{
	return mModel ? mModel->addAction( path, action ) : false;
}

QAction* pActionsNodeMenuBar::addAction( const QString& path, const QString& text, const QIcon& icon )
{
	return mModel ? mModel->addAction( path, text, icon ) : 0;
}

pActionsNode pActionsNodeMenuBar::addMenu( const QString& path )
{
	return mModel ? mModel->addPath( path ) : pActionsNode();
}

bool pActionsNodeMenuBar::removeAction( const QString& path )
{
	return mModel ? mModel->removeAction( path ) : false;
}

bool pActionsNodeMenuBar::removeAction( QAction* action )
{
	return mModel ? mModel->removeAction( action ) : false;
}

bool pActionsNodeMenuBar::removeMenu( const QString& path )
{
	return mModel ? mModel->removePath( path ) : false;
}

QMenu* pActionsNodeMenuBar::menu( const QString& _path ) const
{
	const QString path = pActionsNodeModel::fixedPath( _path );
	return mMenus.value( path );
}

QString pActionsNodeMenuBar::menuPath( QMenu* menu ) const
{
	return mMenus.key( menu );
}

QAction* pActionsNodeMenuBar::action( const QString& path ) const
{
	return mModel ? mModel->pathToNode( path ).action() : 0;
}

QString pActionsNodeMenuBar::actionPath( QAction* action ) const
{
	return mModel ? mModel->actionToNode( action ).path() : QString::null;
}

void pActionsNodeMenuBar::recursiveSync( const pActionsNode& node )
{
	foreach ( const pActionsNode& childNode, node.children() ) {
		model_nodeInserted( childNode );
		
		if ( childNode.hasChildren() ) {
			recursiveSync( childNode );
		}
	}
}

void pActionsNodeMenuBar::sync()
{
	if ( !mModel ) {
		qDeleteAll( mMenus.values() );
		mMenus.clear();
		return;
	}
	
	recursiveSync( mModel->rootNode() );
}

void pActionsNodeMenuBar::model_nodeInserted( const pActionsNode& node )
{
	QMenu* parentMenu = mMenus.value( node.path().section( '/', 0, -2 ) );
	
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
			
			model_nodeChanged( node );
			
			break;
		}
		default:
			break;
	}
}

void pActionsNodeMenuBar::model_nodeChanged( const pActionsNode& node )
{
	switch ( node.type() ) {
		case pActionsNode::Action:
			break;
		case pActionsNode::Path: {
			QMenu* menu = mMenus.value( node.path() );
			
			menu->setIcon( node.icon() );
			menu->setTitle( node.text() );
		}
		default:
			break;
	}
}

void pActionsNodeMenuBar::model_nodeRemoved( const pActionsNode& node )
{
	if ( node.type() == pActionsNode::Path ) {
		QMenu* menu = mMenus.take( node.path() );
		menu->deleteLater();
	}
	else {
		QMenu* parentMenu = mMenus.value( node.path().section( '/', 0, -2 ) );
		parentMenu->removeAction( node.action() );
	}
}

void pActionsNodeMenuBar::model_nodesCleared()
{
	qDeleteAll( mMenus );
	mMenus.clear();
}
