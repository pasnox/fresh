/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsNode.cpp
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
#include "pActionsNode.h"
#include "pActionsNodeModel.h"

uint qHash( const pActionsNode& node )
{
	return qHash( node.path() );
}

// pActionsNode::Data

pActionsNode::Data::Data()
{
	type = pActionsNode::Invalid;
	model = 0;
	parent = 0;
}

pActionsNode::Data::~Data()
{
	if ( model ) {
		model->mNodes.remove( path );
	}
	
	if ( action.data() ) {
		action.data()->deleteLater();
	}
}

bool pActionsNode::Data::operator==( const pActionsNode::Data& other ) const
{
	return type == other.type
		&& path == other.path
		&& action == other.action
		&& icon.cacheKey() == other.icon.cacheKey()
		&& text == other.text
		
		&& model == other.model
		&& parent == other.parent
		&& children == other.children;
}

bool pActionsNode::Data::operator!=( const pActionsNode::Data& other ) const
{
	return !operator==( other );
}

// pActionsNode

pActionsNode::pActionsNode()
{
	d = new pActionsNode::Data;
}

pActionsNode::pActionsNode( const pActionsNode& other )
{
	d = other.d;
}

pActionsNode::pActionsNode( pActionsNode::Type type, const QString& path )
{
	d = new pActionsNode::Data;
	d->type = type;
	d->path = path;
}

bool pActionsNode::operator==( const pActionsNode& other ) const
{
	return d == other.d;
}

bool pActionsNode::operator!=( const pActionsNode& other ) const
{
	return !operator==( other );
}

bool pActionsNode::isValid() const
{
	return d->type != pActionsNode::Invalid;
}

bool pActionsNode::hasChildren() const
{
	return !d->children.isEmpty();
}

pActionsNode::Type pActionsNode::type() const
{
	return d->type;
}

QString pActionsNode::path() const
{
	return d->path;
}

QAction* pActionsNode::action() const
{
	return d->action.data();
}

QIcon pActionsNode::icon() const
{
	switch ( d->type ) {
		case pActionsNode::Action:
			return d->action.data()->icon();
		case pActionsNode::Path:
			return d->icon;
		default:
			return QIcon();
	}
}

QString pActionsNode::text() const
{
	switch ( d->type ) {
		case pActionsNode::Action:
			return d->action.data()->text();
		case pActionsNode::Path:
			return d->text;
		default:
			return QString::null;
	}
}

pActionsNode pActionsNode::parent() const
{
	return d->parent ? *d->parent : pActionsNode();
}

QList<pActionsNode> pActionsNode::children() const
{
	return d->children;
}

void pActionsNode::setIcon( const QIcon& icon )
{
	if ( !d->model ) {
		return;
	}
	
	const QModelIndex index = d->model->nodeToIndex( *this );
	d->model->setData( index, QVariant::fromValue( icon ), Qt::DecorationRole );
}

void pActionsNode::setText( const QString& text )
{
	if ( !d->model ) {
		return;
	}
	
	const QModelIndex index = d->model->nodeToIndex( *this );
	d->model->setData( index, text, Qt::DisplayRole );
}

QKeySequence pActionsNode::shortcut() const
{
	return d->type == pActionsNode::Action
		? ( d->action ? d->action.data()->shortcut() : QKeySequence() )
		: QKeySequence();
}

bool pActionsNode::setShortcut( const QKeySequence& shortcut )
{
	if ( !d->model ) {
		return false;
	}
	
	const QModelIndex index = d->model->nodeToIndex( *this );
	return d->model->setData( index, shortcut, pActionsNode::ShortcutRole );
}
	
QKeySequence pActionsNode::defaultShortcut() const
{
	return d->type == pActionsNode::Action
		? ( d->action ? d->action.data()->data().value<QKeySequence>() : QKeySequence() )
		: QKeySequence();
}

void pActionsNode::setDefaultShortcut( const QKeySequence& shortcut )
{
	if ( !d->model ) {
		return;
	}
	
	const QModelIndex index = d->model->nodeToIndex( *this );
	if ( d->model->setData( index, shortcut, pActionsNode::DefaultShortcutRole ) ) {
		if ( this->shortcut().isEmpty() ) {
			setShortcut( shortcut );
		}
	}
}

pActionsNode pActionsNode::pathNode( const QString& path, const QString& text, const QIcon& icon )
{
	pActionsNode node( pActionsNode::Path, path );
	node.d->icon = icon;
	node.d->text = text;
	
	if ( node.d->text.isEmpty() ) {
		node.d->text = path.section( '/', -1, -1 );
	}
	
	return node;
}

pActionsNode pActionsNode::actionNode( const QString& path, QAction* action )
{
	pActionsNode node( pActionsNode::Action, path );
	node.d->action = action;
	action->setObjectName( path );
	
	return node;
}
